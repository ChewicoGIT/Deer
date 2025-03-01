#include "ActiveEntity.h"

namespace Deer {
    namespace ActiveEntity {
        std::vector<Entity*> entities;
    }

    void ActiveEntity::addEntity(Entity& entt) {
        if (!contains(entt))
            entities.push_back(&entt);
    }

    void ActiveEntity::removeEntity(Entity& entt) {
        auto it = std::remove(entities.begin(), entities.end(), &entt);

        if (it != entities.end())
            entities.erase(it, entities.end());
	}

    bool ActiveEntity::contains(Entity& entity) {
        auto it = std::find(entities.begin(), entities.end(), &entity);

        return it != entities.end();

    }
}
