#include "DeerRender/GizmoRenderer.h"
#include "DeerRender/Render/Render.h"
#include "Deer/Voxel.h"
#include "DeerRender/LightVoxel.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/SceneCamera.h"
#include "Deer/Components.h"
#include "DeerRender/Render/Texture.h"
#include "DeerRender/Render/RenderCommand.h"


namespace Deer {
	void GizmoRenderer::drawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color) {
		m_lines.push_back({ a, b, color });
	}

	void GizmoRenderer::drawVoxelLine(int _x, int _y, int _z, glm::vec3 color) {
		for (int x = 0; x < 2; x++) {
			for (int y = 0; y < 2; y++) {
				glm::vec3 a = glm::vec3(x + _x, y + _y, 0 + _z);
				glm::vec3 b = glm::vec3(x + _x, y + _y, 1 + _z);

				drawLine(a, b, color);
			}
		}
		for (int z = 0; z < 2; z++) {
			for (int y = 0; y < 2; y++) {
				glm::vec3 a = glm::vec3(0 + _x, y + _y, z + _z);
				glm::vec3 b = glm::vec3(1 + _x, y + _y, z + _z);

				drawLine(a, b, color);
			}
		}
		for (int x = 0; x < 2; x++) {
			for (int z = 0; z < 2; z++) {
				glm::vec3 a = glm::vec3(x + _x, 0 + _y, z + _z);
				glm::vec3 b = glm::vec3(x + _x, 1 + _y, z + _z);

				drawLine(a, b, color);
			}
		}

	}

	void GizmoRenderer::drawVoxelLineFace(int x, int y, int z, uint8_t face, glm::vec3 color) {
		glm::vec3 points[4];

		for (int i = 0; i < 4; i++) {
			points[i] = {
				x + NORMAL_VERTEX_POS(X_AXIS, i, face),
				y + NORMAL_VERTEX_POS(Y_AXIS, i, face),
				z + NORMAL_VERTEX_POS(Z_AXIS, i, face)
			};
		}

		drawLine(points[0], points[1], color);
		drawLine(points[2], points[3], color);
		drawLine(points[0], points[2], color);
		drawLine(points[1], points[3], color);
	}

	void GizmoRenderer::drawVoxelFace(int x, int y, int z, uint16_t voxelID, uint8_t faceID, uint8_t priority) {
		glm::vec3 points[4];
		VoxelAspect& aspect = VoxelData::voxelsAspect[voxelID];
		GizmoFace face;
		face.textureID = aspect.textureFacesIDs[faceID];
		face.face = faceID;
		
		for (int i = 0; i < 4; i++) {
			face.positions[i] = {
				x + NORMAL_VERTEX_POS(X_AXIS, i, faceID),
				y + NORMAL_VERTEX_POS(Y_AXIS, i, faceID),
				z + NORMAL_VERTEX_POS(Z_AXIS, i, faceID)
			};
		}

		m_faces[priority].push_back(face);
	}

	void GizmoRenderer::drawVoxelFaceInverted(int x, int y, int z, uint16_t voxelID, uint8_t faceID, uint8_t priority) {
		glm::vec3 points[4];
		VoxelAspect& aspect = VoxelData::voxelsAspect[voxelID];
		GizmoFace face;
		face.textureID = aspect.textureFacesIDs[faceID];
		face.face = faceID;

		face.positions[0] = {
			x + NORMAL_VERTEX_POS(X_AXIS, 0, faceID),
			y + NORMAL_VERTEX_POS(Y_AXIS, 0, faceID),
			z + NORMAL_VERTEX_POS(Z_AXIS, 0, faceID)
		};

		face.positions[2] = {
			x + NORMAL_VERTEX_POS(X_AXIS, 1, faceID),
			y + NORMAL_VERTEX_POS(Y_AXIS, 1, faceID),
			z + NORMAL_VERTEX_POS(Z_AXIS, 1, faceID)
		};

		face.positions[1] = {
			x + NORMAL_VERTEX_POS(X_AXIS, 2, faceID),
			y + NORMAL_VERTEX_POS(Y_AXIS, 2, faceID),
			z + NORMAL_VERTEX_POS(Z_AXIS, 2, faceID)
		};

		face.positions[3] = {
			x + NORMAL_VERTEX_POS(X_AXIS, 3, faceID),
			y + NORMAL_VERTEX_POS(Y_AXIS, 3, faceID),
			z + NORMAL_VERTEX_POS(Z_AXIS, 3, faceID)
		};

		m_faces[priority].push_back(face);
	}

	void GizmoRenderer::render(const SceneCamera& camera) {

		RenderCommand::setDepthBuffer(true);

		// We make a exact camera but with less far clip to give priority in the depht test
		CameraComponent camera_lessDistance = camera.camera;
		camera_lessDistance.farZ *= 1.1f;

		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera_lessDistance.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;

		VoxelData::getVoxelColorTextureAtlas()->bind(0);
		RenderUtils::m_faceShader->bind();
		RenderUtils::m_faceShader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
		RenderUtils::m_faceShader->uploadUniformInt("u_texture", 0);
		RenderUtils::m_faceShader->uploadUniformInt("u_textureSize", VoxelData::getVoxelTextureAtlasSize());

		for (int i = 0; i < GIZMO_DEPTH; i++) {
			for (GizmoFace& face : m_faces[i]) {
				RenderUtils::m_faceShader->uploadUniformInt("u_textureID", face.textureID);
	
				RenderUtils::m_faceShader->uploadUniformFloat3("u_posA", face.positions[0]);
				RenderUtils::m_faceShader->uploadUniformFloat3("u_posB", face.positions[1]);
				RenderUtils::m_faceShader->uploadUniformFloat3("u_posC", face.positions[2]);
				RenderUtils::m_faceShader->uploadUniformFloat3("u_posD", face.positions[3]);
	
				Render::submit(RenderUtils::m_faceVertexArray);
			}
		}

		RenderCommand::setDepthBuffer(false);

		for (std::array<glm::vec3, 3>&line : m_lines) {
			RenderUtils::m_lineShader->bind();
			RenderUtils::m_lineShader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			RenderUtils::m_lineShader->uploadUniformFloat3("u_color", line[2]);

			RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", line[0]);
			RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", line[1]);

			Render::submitLine(RenderUtils::m_lineVertexArray);
		}
	}

	void GizmoRenderer::refresh() {
		m_lines.clear();
		for (int i = 0; i < GIZMO_DEPTH; i++) 
			m_faces[i].clear();
	}
}
