#include "Enviroment.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"

#include "Deer/Core/Log.h"

namespace Deer {
	Environment::Environment() {
		clear();
	}

	Environment::~Environment() { }

	void Environment::clear() {
		// Clear all existing entities and map
		m_registry.clear();
		m_entities.clear();

		m_rootEntity = 0;
		m_mainCamera = 0;
		m_idCreationOffset = 0;

		m_rootEntity = pullEntityID();

		entt::entity rootEntity = m_registry.create();
		Entity entity = { rootEntity, this };

		entity.addComponent<TagComponent>("root", m_rootEntity);
		entity.addComponent<RelationshipComponent>();
		entity.addComponent<TransformComponent>();

		entity.m_isRoot = true;
		entity.m_entityUID = m_rootEntity;
		m_entities.insert({ m_rootEntity, entity });
	}

	Entity& Environment::getEntity(uid id) {
		DEER_CORE_ASSERT(m_entities.contains(id), "Entity id : {0} does not exist", id);
		return m_entities[id];
	}
	
	Entity& Environment::createEntity(const std::string& name)
	{
		uid id;
		do {
			id = pullEntityID();
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

	Entity Environment::createEmptyEntity() {
		entt::entity entityID = m_registry.create();
		Entity entity = { entityID, this };

		return entity;
	}

	uid Environment::tryGetMainCamera() {
		return m_mainCamera;
	}

	void Environment::setMainCamera(Entity& entity) {
		if (!entity.isValid())
			m_mainCamera = 0;

		m_mainCamera = entity.m_entityUID;
	}

	Entity& Environment::getRoot() {
		return m_entities[m_rootEntity];
	}
}