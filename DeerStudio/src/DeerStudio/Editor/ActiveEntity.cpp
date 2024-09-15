#include "ActiveEntity.h"

namespace Deer {
    void ActiveEntity::addEntity(Entity& entt) {
        if (!contains(entt))
            m_entitys.push_back(entt);
    }

    void ActiveEntity::removeEntity(Entity& entt) {
        auto it = std::remove(m_entitys.begin(), m_entitys.end(), entt);

        if (it != m_entitys.end())
            m_entitys.erase(it, m_entitys.end());
	}

    bool ActiveEntity::contains(Entity& entity) {
        auto it = std::find(m_entitys.begin(), m_entitys.end(), entity);

        return it != m_entitys.end();

    }
}
