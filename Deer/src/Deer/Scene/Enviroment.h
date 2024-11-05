#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Scene/Components.h"
#include "Deer/Render/FrameBuffer.h"
#include "entt/entt.hpp"

#include <string>
#include <unordered_map>

namespace Deer {
	class Entity;
	using EntityMap = std::unordered_map<uid, Entity>;

	struct VirtualCamera {
		TransformComponent transform;
		CameraComponent camera;
	};

	class Environment {
	public:
		Environment(const std::string& rootName = "root");
		~Environment();

		void clear();

#ifndef DEER_SERVICE
		void render(Entity& camera);
		void render(VirtualCamera& camera);
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

