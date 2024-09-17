#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Render/FrameBuffer.h"
#include "Deer/Scene/Components.h"
#include "entt/entt.hpp"

#include <string>
#include <unordered_map>

namespace Deer {
	class Entity;

	struct VirtualCamera {
		TransformComponent transform;
		CameraComponent camera;
	};

	class Environment {
	public:
		Environment(const std::string& rootName = "root");
		~Environment();

		void render(Entity& camera);
		void render(VirtualCamera& camera);

		Entity tryGetEntity(uid id);
		Entity createEntity(const std::string& name = std::string());

		Entity getRoot();
	public:
		entt::registry m_registry;
		entt::entity m_rootEntity;

		friend class Entity;
	};
}

