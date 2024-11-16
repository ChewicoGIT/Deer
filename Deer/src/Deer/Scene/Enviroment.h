#pragma once
#include "Deer/Core/Core.h"
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
		Environment(const std::string& rootName = "root");
		~Environment();

		void clear();

#ifdef DEER_RENDER
		void render(SceneCamera& camera);
#endif

		Entity& getEntity(uid id);
		Entity& createEntity(const std::string& name = std::string());
		Entity& createEmptyEntity();

		uid tryGetMainCamera();
		void setMainCamera(Entity& entity);

		Entity& getRoot();
	public:
		const std::string m_rootName;
		entt::registry m_registry;

		Entity* m_rootEntity;
		EntityMap m_entities;

		uid m_idCreationOffset = 0;
		uid m_mainCamera = 0;

		friend class Entity;
	};
}

