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

		uid id = m_idCreationOffset + 1;
		m_idCreationOffset++;

		entt::entity entityID = m_registry.create();
		Entity entity = { entityID, this };
		entity.addComponent<TagComponent>(rootName, id);
		entity.addComponent<RelationshipComponent>();
		entity.addComponent<TransformComponent>();

		entity.m_isRoot = true;
		entity.m_entityUID = id;
		m_entities.insert({ id, entity });

		auto& rootEntity = m_entities[id];
		m_rootEntity = &rootEntity;
	}

	Environment::~Environment() { }

	void Environment::clear() {
		m_registry.clear();
		m_entities.clear();
		m_rootEntity = nullptr;
		m_idCreationOffset = 0;
	}

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

		auto view = m_registry.view<MeshRenderComponent, TagComponent>();
		for (auto entityId : view) {
			auto& meshRender = view.get<MeshRenderComponent>(entityId);
			if (meshRender.shaderAssetID == 0)
				continue;

			if (meshRender.meshAssetID == 0)
				continue;

			auto& tag = view.get<TagComponent>(entityId);
			Entity& entity = tryGetEntity(tag.entityUID);

			glm::mat4 matrix = entity.getWorldMatrix();
			Asset<Shader>& shaderAsset = Project::m_assetManager.getAsset<Shader>(meshRender.shaderAssetID);
			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", matrix);
			shaderAsset.value->uploadUniformInt("u_objectID", tag.entityUID);

			shaderAsset.value->bind();

			Asset<Mesh>& meshAsset = Project::m_assetManager.getAsset<Mesh>(meshRender.meshAssetID);
			meshAsset.value->bind();

			Render::submit(meshAsset.value);
		}
	}

	Entity& Environment::tryGetEntity(uid id) {
		return m_entities[id];
	}
	
	Entity& Environment::createEntity(const std::string& name)
	{
		uid id;
		do {
			id = m_idCreationOffset + 1;
			m_idCreationOffset++;
		} while (m_entities.contains(id));

		entt::entity entityID = m_registry.create();
		Entity entity = { entityID, this };
		entity.addComponent<TagComponent>(name, id);
		entity.addComponent<RelationshipComponent>();
		entity.addComponent<TransformComponent>();

		entity.m_entityUID = id;
		entity.setParent(getRoot());

		m_entities.insert({ id, entity });
		return m_entities[id];
	}

	Entity& Environment::createEmptyEntity()
	{
		entt::entity entityID = m_registry.create();
		Entity entity = { entityID, this };

		return entity;
	}

	Entity& Environment::getRoot() {
		return *m_rootEntity;
	}

}