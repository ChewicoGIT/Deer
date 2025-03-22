#pragma once

#include "Deer/Components.h"
#include "entt/entt.hpp"

#include <string>
#include <unordered_map>

#ifdef DEER_RENDER
#include "DeerRender/SceneCamera.h"
#include "DeerRender/Render/FrameBuffer.h"
#endif

namespace Deer {
	class Entity;
	using EntityMap = std::unordered_map<uint32_t, Entity>;

	class Environment {
	public:
		Environment();
		~Environment();

		void clear();

#ifdef DEER_RENDER
		void render(SceneCamera& camera);
#endif

		Entity& getEntity(uint32_t id);
		Entity& createEntity(const std::string& name = std::string());
		Entity createEmptyEntity();
		
		//FEO
		uint32_t tryGetMainCamera();
		void setMainCamera(Entity& entity);

		Entity& getRoot();
	public:
		entt::registry m_registry;
		EntityMap m_entities;

		uint32_t m_rootEntity = 0;
		uint32_t m_mainCamera = 0;
	private:
		uint32_t m_idCreationOffset = 0;
		inline uint32_t pullEntityID() { m_idCreationOffset++; return m_idCreationOffset; }

		friend class Entity;
	};
}

