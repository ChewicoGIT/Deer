#pragma once
#include "Deer/Scene/Entity.h"

#include <vector>

namespace Deer {
	namespace ActiveEntity {
		extern std::vector<Entity*> entities;

		void addEntity(Entity& entt);
		void removeEntity(Entity& entt);

		inline Entity& getEntity(unsigned int pos) { return *entities[pos]; }
		bool contains(Entity& entity);

		inline void clear() { entities.clear(); }
		inline int count() { return entities.size(); }

		template <typename T>
		inline bool shareComponent() {
			for (auto& entity : entities) {
				if (!entity->hasComponent<T>())
					return false;
			}

			return true;
		}
	}
}

