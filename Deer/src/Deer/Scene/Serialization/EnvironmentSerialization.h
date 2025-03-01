#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Scene/Enviroment.h"

#include <vector>

namespace Deer {
    struct EntityVector_Environment {
        std::vector<Entity> entities;
        const Ref<Environment>& environment;
        EntityVector_Environment(const Ref<Environment>& _environment)
            : environment(_environment) { }
    };

    template<class Archive>
    void save(Archive& archive,
        Ref<Environment> const& m_environment) {
        EntityVector_Environment entityMap(m_environment);
        auto view = m_environment->m_registry.view<TagComponent>();

        for (auto entity : view) {
            TagComponent& tag = view.get<TagComponent>(entity);
            entityMap.entities.push_back(m_environment->getEntity(tag.entityUID));
        }

        // Sort to avoid conflicts
        std::sort(entityMap.entities.begin(), entityMap.entities.end(), [](Entity& a, Entity& b) {
            return a.getUID() < b.getUID();
            });

        uid mainCameraUID = m_environment->tryGetMainCamera();

        archive(cereal::make_nvp("entities", entityMap));
        archive(cereal::make_nvp("mainCameraUID", mainCameraUID));

    }

    template<class Archive>
    void load(Archive& archive,
        Ref<Environment>& m_environment) {
        EntityVector_Environment entityMap(m_environment);

        uid mainCameraUID;

        archive(cereal::make_nvp("entities", entityMap));
        archive(cereal::make_nvp("mainCameraUID", mainCameraUID));

        if (mainCameraUID != 0)
            m_environment->setMainCamera(m_environment->getEntity(mainCameraUID));

    }

    // ENVIRONMENT
    template<class Archive>
    void save(Archive& archive,
        EntityVector_Environment const& m_entityVector) {

        archive(cereal::make_size_tag(static_cast<cereal::size_type>(m_entityVector.entities.size()))); // number of elements
        for (auto&& v : m_entityVector.entities)
            archive(v);
    }

    template<class Archive>
    void load(Archive& archive,
        EntityVector_Environment& m_entityVector) {

        cereal::size_type size;
        archive(cereal::make_size_tag(size));

        m_entityVector.entities.resize(static_cast<std::size_t>(size));
        for (auto& v : m_entityVector.entities) {
            v = m_entityVector.environment->createEmptyEntity();
            archive(v);
        }
    }
}