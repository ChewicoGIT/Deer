#include "ActiveEntity.h"

namespace Deer {
    void ActiveEntity::addEntity(Entity& entt) {
        if (!contains(entt))
            m_entites.push_back(entt);
    }

    void ActiveEntity::removeEntity(Entity& entt) {
        auto it = std::remove(m_entites.begin(), m_entites.end(), entt);

        if (it != m_entites.end())
            m_entites.erase(it, m_entites.end());
	}

    bool ActiveEntity::contains(Entity& entity) {
        auto it = std::find(m_entites.begin(), m_entites.end(), entity);

        return it != m_entites.end();

    }
}
