#include "Enviroment.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "Deer/Render/Render.h"

#include "Deer/Core/Log.h"

namespace Deer {
	Environment::Environment(const std::string& rootName) {
		DEER_CORE_TRACE("Creating enviroment with root : {0}", rootName.c_str());
		auto rootEntity = new Entity(m_registry.create(), this);

		rootEntity->addComponent<TagComponent>(rootName.c_str());
		rootEntity->addComponent<RelationshipComponent>();
		rootEntity->addComponent<TransformComponent>();
		rootEntity->setRoot(true);

		m_rootEntity = rootEntity->m_entityHandle;
	}

	Environment::~Environment() { }

	void Environment::render(Entity& camera) {
		DEER_CORE_ASSERT(camera.isValid(), "Rendering camera is not valid");
		
		auto& cameraComponent = camera.getComponent<CameraComponent>();
		
		glm::mat4 camMatrix = glm::inverse(camera.getWorldMatrix());
		glm::mat4 projectionMatrix = cameraComponent.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));
		glm::mat4 invertY = glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1));
		
		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = invertY * projectionMatrix * invertZ * camMatrix;

		auto view = m_registry.view<MeshRenderComponent>();
		for (auto entityId : view) {
			auto meshRender = view.get<MeshRenderComponent>(entityId);
			if (meshRender.shaderAssetID == 0)
				continue;

			if (meshRender.meshAssetID == 0)
				continue;

			Entity entity = tryGetEntity((uid)entityId);

			glm::mat4 matrix = entity.getWorldMatrix();
			Asset<Shader>& shaderAsset = Project::m_assetManager.getAsset<Shader>(meshRender.shaderAssetID);
			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", matrix);
			shaderAsset.value->uploadUniformMat4("u_objectID", (int)entityId);

			shaderAsset.value->bind();

			Asset<Mesh>& meshAsset = Project::m_assetManager.getAsset<Mesh>(meshRender.meshAssetID);
			meshAsset.value->bind();

			Render::submit(meshAsset.value);
		}
	}

	void Environment::render(VirtualCamera& camera) {

		glm::mat4 camMatrix = glm::inverse(camera.transform.getMatrix());
		glm::mat4 projectionMatrix = camera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));
		glm::mat4 invertY = glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1));

		// Lets invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = invertY * projectionMatrix * invertZ * camMatrix;

		auto view = m_registry.view<MeshRenderComponent>();
		for (auto entityId : view) {
			auto meshRender = view.get<MeshRenderComponent>(entityId);
			if (meshRender.shaderAssetID == 0)
				continue;

			if (meshRender.meshAssetID == 0)
				continue;

			Entity entity = tryGetEntity((uid)entityId);

			glm::mat4 matrix = entity.getWorldMatrix();
			Asset<Shader>& shaderAsset = Project::m_assetManager.getAsset<Shader>(meshRender.shaderAssetID);
			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", matrix);
			shaderAsset.value->uploadUniformInt("u_objectID", (int)entityId);

			shaderAsset.value->bind();

			Asset<Mesh>& meshAsset = Project::m_assetManager.getAsset<Mesh>(meshRender.meshAssetID);
			meshAsset.value->bind();

			Render::submit(meshAsset.value);
		}
	}

	Entity Environment::tryGetEntity(uid id) {
		if (m_registry.valid((entt::entity)id))
			return Entity((entt::entity)id, this);
		return Entity();
	}
	
	Entity Environment::createEntity(const std::string& name)
	{
		Entity entity = { m_registry.create(), this };
		entity.addComponent<TagComponent>(name);
		entity.addComponent<RelationshipComponent>();
		entity.addComponent<TransformComponent>();

		Entity root = getRoot();
		entity.setParent(root);

		return entity;
	}

	Entity Environment::getRoot() {
		return Entity(m_rootEntity, this);
	}

}