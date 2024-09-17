#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Log.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Entity.h"

#include "entt/entt.hpp"

#include <vector>
#include <algorithm>

namespace Deer {
	class Entity {
	public:
		Entity() { }

		template <typename T, typename...Args>
		T& addComponent(Args&&... args) {
			DEER_CORE_ASSERT(!m_environment->m_registry.all_of<T>(m_entityHandle),
				"Entity already have component {0}", typeid(T).name());

			return m_environment->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent() {
			DEER_CORE_ASSERT(m_environment->m_registry.all_of<T>(m_entityHandle),
				"Entity has no component {0}", typeid(T).name());

			return m_environment->m_registry.get<T>(m_entityHandle);
		}

		template<typename T>
		bool hasComponent() {
			return m_environment->m_registry.all_of<T>(m_entityHandle);
		}

		template<typename T>
		void removeComponent() {
			DEER_CORE_ASSERT(m_environment->m_registry.all_of<T>(m_entityHandle),
				"Entity does not have component {0}", typeid(T).name());

			m_environment->m_registry.remove<T>(m_entityHandle);
		}

		Entity duplicate();
		void destroy();

		Entity getParent();
		// To do, enable transfer entitys from difrent enviroments
		void setParent(Entity& parent);
		bool isDescendant(Entity& parent);

		entt::entity getParentUID();
		uid getUID() const { return (uid)m_entityHandle; }
		void setParentUID(entt::entity parentUid);
		std::vector<entt::entity>& getChildren();

		bool isRoot() { return getComponent<RelationshipComponent>().root; }
		glm::mat4 getWorldMatrix();
		glm::mat4 getRelativeMatrix();

		inline bool isValid() const { return m_environment != nullptr && m_environment->m_registry.valid(m_entityHandle); }
		inline bool operator== (const Entity& b) const { return m_environment == b.m_environment && m_entityHandle == b.m_entityHandle; }
	private:
		Entity(entt::entity handle, Environment* scene);
		bool removeChild(Entity& child);

		void setRoot(bool value) { getComponent<RelationshipComponent>().root = value; }
		
		entt::entity m_entityHandle = entt::null;
		Environment* m_environment = nullptr;

		friend class Environment;
	};
}

