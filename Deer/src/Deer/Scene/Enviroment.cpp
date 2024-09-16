#include "Enviroment.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "Deer/Render/Render.h"

#include "Deer/Core/Log.h"

namespace Deer {
	Environment::Environment(const std::string& rootName) {
		DEER_CORE_TRACE("Creating enviroment with root : {0}", rootName.c_str());
		m_rootEntity = new Entity(m_registry.create(), this);

		m_rootEntity->addComponent<TagComponent>(rootName.c_str());
		m_rootEntity->addComponent<RelationshipComponent>();
		m_rootEntity->addComponent<TransformComponent>();
		m_rootEntity->setRoot(true);

		m_camera = Scope<Entity>(new Entity());
	}

	Environment::~Environment() {
		DEER_CORE_TRACE("Destroying enviroment with root : {0}", m_rootEntity->getComponent<TagComponent>().tag.c_str());
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
			if (meshRender.mesh == nullptr || meshRender.shader == nullptr)
				continue;
			
			Entity entity = tryGetEntity((uid)entityId);
			
			glm::mat4 matrix = entity.getWorldMatrix();
			meshRender.shader->bind();
			meshRender.shader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			meshRender.shader->uploadUniformMat4("u_worldMatrix", matrix);

			meshRender.shader->bind();
			meshRender.mesh->bind();

			Render::submit(meshRender.mesh);
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
			if (meshRender.mesh == nullptr || meshRender.shader == nullptr)
				continue;

			Entity entity = tryGetEntity((uid)entityId);

			glm::mat4 matrix = entity.getWorldMatrix();
			meshRender.shader->bind();
			meshRender.shader->uploadUniformMat4("u_viewMatrix", cameraProjectionMatrix);
			meshRender.shader->uploadUniformMat4("u_worldMatrix", matrix);

			meshRender.shader->bind();
			meshRender.mesh->bind();

			Render::submit(meshRender.mesh);
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

		entity.setParent(*m_rootEntity);

		return entity;
	}

	void Environment::setMainCamera(Entity& entity) { *m_camera = entity; }
}