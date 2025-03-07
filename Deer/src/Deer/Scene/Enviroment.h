#pragma once

#include "Deer/Scene/Components.h"
#include "entt/entt.hpp"

#include <string>
#include <unordered_map>

#ifdef DEER_RENDER
#include "DeerRender/Scene/SceneCamera.h"
#include "DeerRender/Render/FrameBuffer.h"
#endif

namespace Deer {
	class Entity;
	using EntityMap = std::unordered_map<uid, Entity>;

	class Environment {
	public:
		Environment();
		~Environment();

		void clear();

#ifdef DEER_RENDER
		void render(SceneCamera& camera);
#endif

		Entity& getEntity(uid id);
		Entity& createEntity(const std::string& name = std::string());
		Entity createEmptyEntity();
		
		//FEO
		uid tryGetMainCamera();
		void setMainCamera(Entity& entity);

		Entity& getRoot();
	public:
		entt::registry m_registry;
		EntityMap m_entities;

		uid m_rootEntity = 0;
		uid m_mainCamera = 0;
	private:
		uid m_idCreationOffset = 0;
		inline uid pullEntityID() { m_idCreationOffset++; return m_idCreationOffset; }

		friend class Entity;
	};
}

