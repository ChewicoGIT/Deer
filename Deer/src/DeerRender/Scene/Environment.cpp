#include "Deer/Scene/Enviroment.h"

#include "Deer/Application.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"
#include "Deer/Voxel.h"

#include "Deer/Core/Log.h"

namespace Deer {
	void Environment::render(SceneCamera& camera) {
		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;
		{
			auto view = m_registry.view<MeshRenderComponent, TagComponent>();
			for (auto entityId : view) {
				auto& meshRender = view.get<MeshRenderComponent>(entityId);
				if (meshRender.shaderAssetID == 0)
					continue;

				if (meshRender.meshAssetID == 0)
					continue;

				auto& tag = view.get<TagComponent>(entityId);
				Entity& entity = getEntity(tag.entityUID);

				if (entity.hasComponent<TextureBindingComponent>()) {
					TextureBindingComponent& textureBinding = entity.getComponent<TextureBindingComponent>();

					for (int x = 0; x < MAX_TEXTURE_BINDINGS; x++) {
						if (textureBinding.textureAssetID[x] == 0)
							continue;

						Asset<Texture2D>& textureAsset = AssetManager::getAsset<Texture2D>(textureBinding.textureAssetID[x]);
						textureAsset.value->bind(textureBinding.textureBindID[x]);
					}
				}

				glm::mat4 matrix = entity.getWorldMatrix();
				Asset<Shader>& shaderAsset = AssetManager::getAsset<Shader>(meshRender.shaderAssetID);
				shaderAsset.value->bind();
				shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
				shaderAsset.value->uploadUniformMat4("u_worldMatrix", matrix);
				shaderAsset.value->uploadUniformInt("u_objectID", tag.entityUID);

				Asset<Mesh>& meshAsset = AssetManager::getAsset<Mesh>(meshRender.meshAssetID);
				meshAsset.value->bind();

				Render::submit(meshAsset.value);
			}
		}

		// Draw Grid Gizmo
		{
			RenderUtils::m_lineShader->bind();
			RenderUtils::m_lineShader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			RenderUtils::m_lineShader->uploadUniformFloat3("u_color", glm::vec3(.5f, .5f, .5f));
			for (int x = 0; x < CHUNK_SIZE_X + 1; x++) {
				RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", glm::vec3(x, 0, 0));
				RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", glm::vec3(x, 0, CHUNK_SIZE_Z));

				Render::submitLine(RenderUtils::m_lineVertexArray);
			}
			for (int z = 0; z < CHUNK_SIZE_Z + 1; z++) {
				RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", glm::vec3(0, 0, z));
				RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", glm::vec3(CHUNK_SIZE_X, 0, z));

				Render::submitLine(RenderUtils::m_lineVertexArray);
			}
		}

		// Rendering Camera Gizmo
		{
			RenderUtils::m_lineShader->bind();
			RenderUtils::m_lineShader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			RenderUtils::m_lineShader->uploadUniformFloat3("u_color", glm::vec3(.7f, .85f, 1));

			auto view = m_registry.view<CameraComponent, TagComponent>();
			for (auto entityId : view) {
				CameraComponent cameraComponent = view.get<CameraComponent>(entityId);
				cameraComponent.nearZ = .5f;
				cameraComponent.farZ = 2;
				TagComponent& tag = view.get<TagComponent>(entityId);

				Entity& entity = getEntity(tag.entityUID);

				glm::mat4 matrix = entity.getWorldMatrix();
				glm::mat4 camPrespective = glm::inverse(cameraComponent.getMatrix());

				glm::mat4 cameraMatrix = camPrespective;
				glm::vec3 cameraAxisPoints[2 * 2 * 2];

				// Generate 8 Points
				for (int x = 0; x < 2; x++) {
					for (int y = 0; y < 2; y++) {
						for (int z = 0; z < 2; z++) {
							glm::vec4 endPos = invertZ * camPrespective * glm::vec4(x * 2 - 1, y * 2 - 1, z, 1);
							endPos = endPos * endPos.w;
							endPos.w = 1;

							cameraAxisPoints[z * 4 + y * 2 + x] = matrix * endPos;
						}
					}
				}

				// Draw the lines
				for (int x = 0; x < 2; x++) {
					for (int y = 0; y < 2; y++) {
						int posA = 0 * 4 + y * 2 + x;
						int posB = 1 * 4 + y * 2 + x;

						RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", cameraAxisPoints[posA]);
						RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", cameraAxisPoints[posB]);

						Render::submitLine(RenderUtils::m_lineVertexArray);
					}
				}

				for (int x = 0; x < 2; x++) {
					for (int z = 0; z < 2; z++) {
						int posA = z * 4 + 0 * 2 + x;
						int posB = z * 4 + 1 * 2 + x;

						RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", cameraAxisPoints[posA]);
						RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", cameraAxisPoints[posB]);

						Render::submitLine(RenderUtils::m_lineVertexArray);
					}
				}

				for (int y = 0; y < 2; y++) {
					for (int z = 0; z < 2; z++) {
						int posA = z * 4 + y * 2 + 0;
						int posB = z * 4 + y * 2 + 1;

						RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", cameraAxisPoints[posA]);
						RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", cameraAxisPoints[posB]);

						Render::submitLine(RenderUtils::m_lineVertexArray);
					}
				}
			}
		}


	}
}