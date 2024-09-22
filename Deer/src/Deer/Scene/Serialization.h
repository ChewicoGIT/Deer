#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Scene.h"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "entt/entt.hpp"

#include <vector>

namespace cereal {
    template<class Archive>
    void serialize(Archive& archive,
        glm::vec3& vec3) {

        archive(
            cereal::make_nvp("x", vec3.x),
            cereal::make_nvp("y", vec3.y),
            cereal::make_nvp("z", vec3.z));

    }

    template<class Archive>
    void serialize(Archive& archive,
        glm::quat& rot) {

        archive(cereal::make_nvp("x", rot.x));
        archive(cereal::make_nvp("y", rot.y));
        archive(cereal::make_nvp("z", rot.z));
        archive(cereal::make_nvp("w", rot.w));
    }

}

namespace Deer {
    template<class Archive>
    void save(Archive& archive,
        MeshRenderComponent const& meshRender) {

        //std::string meshLocation = Project::m_assetManager.getAssetLocation(meshRender.meshAssetID);
        //archive(cereal::make_nvp("mesh", meshLocation));
        //std::string shaderLocation = Project::m_assetManager.getAssetLocation(meshRender.shaderAssetID);
        //archive(cereal::make_nvp("shader", shaderLocation));
    }

    template<class Archive>
    void load(Archive& archive,
        MeshRenderComponent& meshRender) {

    }

    template<class Archive>
    void serialize(Archive& archive,
        TransformComponent& transform) {

        archive(cereal::make_nvp("scale", transform.position));
        archive(cereal::make_nvp("scale", transform.scale));
        archive(cereal::make_nvp("rotation", transform.rotation));
        
    }

    template<class Archive>
    void save(Archive& archive,
        Entity const& m_entity) {

        uid id = m_entity.getUID();
        TagComponent& name = m_entity.getComponent<TagComponent>();
        archive(cereal::make_nvp("id", id));
        archive(cereal::make_nvp("name", name.tag));

        TransformComponent& transform = m_entity.getComponent<TransformComponent>();
        archive(cereal::make_nvp("transform", transform));

        bool hasMeshRenderComponent = m_entity.hasComponent<MeshRenderComponent>();
        archive(cereal::make_nvp("hasMeshRenderComponent", hasMeshRenderComponent));
        if (hasMeshRenderComponent) {
            MeshRenderComponent& meshRender = m_entity.getComponent<MeshRenderComponent>()
            archive(cereal::make_nvp("meshRenderComponent", meshRender));
        }

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