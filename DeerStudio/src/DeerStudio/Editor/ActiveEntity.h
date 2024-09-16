#pragma once
#include "Deer/Scene/Entity.h"

#include <vector>

namespace Deer {
	class ActiveEntity {
	public:
		void addEntity(Entity& entt);
		void removeEntity(Entity& entt);

		Entity& getEntity(unsigned int pos) { return m_entites[pos]; }
		bool contains(Entity& entity);
		
		void clear() { m_entites.clear(); }
		int count() { return m_entites.size(); }

		template <typename T>
		bool shareComponent() {
			for (auto &entity : m_entites) {
				if (!entity.hasComponent<T>())
					return false;
			}

			return true;
		}

		std::vector<Entity>::iterator begin() { return m_entites.begin(); }
		std::vector<Entity>::iterator end() { return m_entites.end(); }

	private:
		std::vector<Entity> m_entites;
	};
}

