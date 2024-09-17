#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Scene.h"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "entt/entt.hpp"

#include <vector>

namespace Deer {

    template<class Archive>
    void save(Archive& archive,
        Entity const& m_entity) {

        uid id = m_entity.getUID();
        archive(cereal::make_nvp("id", id));
    }

    template<class Archive>
    void load(Archive& archive,
        Entity& m_entity) {

        uid id;
        archive(cereal::make_nvp("id", &id));
    }


    template<class Archive>
    void save(Archive& archive,
        Ref<Environment> const& m_environment) {
        std::vector<Entity> entityMap;

        auto view = m_environment->m_registry.view<entt::entity>();

        for (auto entity : view) {
            entityMap.push_back(m_environment->tryGetEntity((uid)entity));
        }

        archive(cereal::make_nvp("entities", entityMap));
    }

    template<class Archive>
    void load(Archive& archive,
        Ref<Environment> & m_environment) {
        std::vector<Entity> entityMap;

        archive(cereal::make_nvp("entities", entityMap));
    }

    template<class Archive>
    void serialize(Archive& archive,
        Ref<Scene> & m_scene) {

        archive(cereal::make_nvp("main_environment", m_scene->getMainEnviroment()));
    }
}