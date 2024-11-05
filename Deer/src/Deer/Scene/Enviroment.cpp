#include "Enviroment.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "Deer/Render/Render.h"
#include "Deer/Render/RenderUtils.h"
#include "Deer/Render/Texture.h"

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
		m_mainCamera = 0;

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
		if (!entity.isValid())
			m_mainCamera = 0;

		m_mainCamera = entity.m_entityUID;
	}

	Entity& Environment::getRoot() {
		return *m_rootEntity;
	}

}