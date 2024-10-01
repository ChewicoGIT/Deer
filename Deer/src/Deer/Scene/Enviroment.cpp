#include "Enviroment.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "Deer/Render/Render.h"
#include "Deer/Render/RenderUtils.h"

#include "Deer/Core/Log.h"

namespace Deer {
	Environment::Environment(const std::string& rootName) 
		: m_rootName(rootName) {
		DEER_CORE_TRACE("Creating enviroment with root : {0}", rootName.c_str());

		clear();
	}

	Environment::~Environment() { }

	void Environment::clear() {
		m_registry.clear();
		m_entities.clear();
		m_rootEntity = nullptr;
		m_idCreationOffset = 0;

		uid id = m_idCreationOffset + 1;
		m_idCreationOffset++;

		entt::entity entityID = m_registry.create();
		Entity entity = { entityID, this };
		entity.addComponent<TagComponent>(m_rootName, id);
		entity.addComponent<RelationshipComponent>();
		entity.addComponent<TransformComponent>();

		entity.m_isRoot = true;
		entity.m_entityUID = id;
		m_entities.insert({ id, entity });

		auto& rootEntity = m_entities[id];
		m_rootEntity = &rootEntity;
	}

	void Environment::render(Entity& camera) {
		DEER_CORE_ASSERT(camera.isValid(), "Rendering camera is not valid");
		
		CameraComponent& cameraComponent = camera.getComponent<CameraComponent>();
		
		glm::mat4 camMatrix = glm::inverse(camera.getWorldMatrix());
		glm::mat4 projectionMatrix = cameraComponent.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));
		glm::mat4 invertY = glm::scale(glm::mat4(1.0f), glm::vec3(1, -1, 1));
		
		// Invert the z axis for engine convenience
		glm::mat4 cameraProjectionMatrix = invertY * projectionMatrix * invertZ * camMatrix;

		auto view = m_registry.view<MeshRenderComponent, TagComponent>();
		for (auto entityId : view) {
			auto& meshRender = view.get<MeshRenderComponent>(entityId);
			if (meshRender.shaderAssetID == 0)
				continue;

			if (meshRender.meshAssetID == 0)
				continue;

			auto& tag = view.get<TagComponent>(entityId);
			Entity& entity = getEntity(tag.entityUID);

			glm::mat4 matrix = entity.getWorldMatrix();
			Asset<Shader>& shaderAsset = Project::m_assetManager.getAsset<Shader>(meshRender.shaderAssetID);
			shaderAsset.value->bind();
			shaderAsset.value->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			shaderAsset.value->uploadUniformMat4("u_worldMatrix", matrix);

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
			Entity& entity = getEntity(tag.entityUID);

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

		RenderUtils::m_lineShader->bind();
		RenderUtils::m_lineShader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
		RenderUtils::m_lineShader->uploadUniformFloat3("u_posA", glm::vec3(0, 0, 0));
		RenderUtils::m_lineShader->uploadUniformFloat3("u_posB", glm::vec3(0, 10, 0));
		RenderUtils::m_lineShader->uploadUniformFloat3("u_color", glm::vec3(1, 1, 1));

		Render::submitLine(RenderUtils::m_lineVertexArray);

	}

	Entity& Environment::getEntity(uid id) {
		DEER_CORE_ASSERT(m_entities.contains(id), "Entity id : {0} does not exist", id);
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

	uid Environment::tryGetMainCamera() {
		return m_mainCamera;
	}

	void Environment::setMainCamera(Entity& entity) {
		m_mainCamera = entity.m_entityUID;
	}

	Entity& Environment::getRoot() {
		return *m_rootEntity;
	}

}