#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Log.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"

#include "entt/entt.hpp"

#include <vector>
#include <algorithm>
#include <unordered_map>

namespace Deer {
	class Entity {
	public:
		Entity() { }

		static Entity nullEntity;

		template <typename T, typename...Args>
		T& addComponent(Args&&... args) const {
			DEER_CORE_ASSERT(!m_environment->m_registry.all_of<T>(m_entityHandle),
				"Entity already have component {0}", typeid(T).name());

			return m_environment->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() const {
			DEER_CORE_ASSERT(m_environment->m_registry.all_of<T>(m_entityHandle),
				"Entity has no component {0}", typeid(T).name());

			return m_environment->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool hasComponent() const {
			return m_environment->m_registry.all_of<T>(m_entityHandle);
		}

		template<typename T>
		void removeComponent() const {
			DEER_CORE_ASSERT(m_environment->m_registry.all_of<T>(m_entityHandle),
				"Entity does not have component {0}", typeid(T).name());

			m_environment->m_registry.remove<T>(m_entityHandle);
		}

		Entity& duplicate();
		void destroy();

		Entity& getParent();
		// TODO, enable transfer entitys from difrent enviroments
		void setParent(Entity& parent);
		bool isDescendant(Entity& parent);

		uid getParentUID() const { return m_parentUID; }
		uid getUID() const { return m_entityUID; }

		Environment* getEnvironment() const { return m_environment; }
		std::vector<uid>& getChildren();

		bool isRoot() { return m_isRoot; }
		glm::mat4 getWorldMatrix();
		glm::mat4 getRelativeMatrix();

		void updateExecution();

		inline bool isValid() const { return m_entityUID != 0 && m_environment != nullptr && m_environment->m_registry.valid(m_entityHandle); }
		inline bool operator== (const Entity& b) const { return m_environment == b.m_environment && m_entityUID == b.m_entityUID; }
	private:
		Entity(entt::entity handle, Environment* scene);
		bool removeChild(Entity& child);
		
		entt::entity m_entityHandle = entt::null;
		Environment* m_environment = nullptr;
		uid m_entityUID = 0;
		uid m_parentUID = 0;
		bool m_isRoot = false;

		friend class Environment;
		friend class std::unordered_map<uid, Entity>;
	};
}

