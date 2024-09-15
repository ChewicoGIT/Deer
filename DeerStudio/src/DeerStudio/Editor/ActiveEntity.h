#pragma once
#include "Deer/Scene/Entity.h"

#include <vector>

namespace Deer {
	class ActiveEntity {
	public:
		void addEntity(Entity& entt);
		void removeEntity(Entity& entt);

		Entity& getEntity(unsigned int pos) { return m_entitys[pos]; }
		bool contains(Entity& entity);
		
		void clear() { m_entitys.clear(); }
		int count() { return m_entitys.size(); }

		std::vector<Entity>::iterator begin() { return m_entitys.begin(); }
		std::vector<Entity>::iterator end() { return m_entitys.end(); }

	private:
		std::vector<Entity> m_entitys;
	};
}

