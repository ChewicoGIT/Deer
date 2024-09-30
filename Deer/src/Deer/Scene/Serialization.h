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

        std::string meshLocation = Project::m_assetManager.getAssetLocation(meshRender.meshAssetID).string();
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation = Project::m_assetManager.getAssetLocation(meshRender.shaderAssetID).string();
        archive(cereal::make_nvp("shader", shaderLocation));
    }

    template<class Archive>
    void load(Archive& archive,
        MeshRenderComponent& meshRender) {

        std::string meshLocation;
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation;
        archive(cereal::make_nvp("shader", shaderLocation));

        meshRender.meshAssetID = Project::m_assetManager.loadAsset<Mesh>(std::filesystem::path(meshLocation));
        meshRender.shaderAssetID = Project::m_assetManager.loadAsset<Shader>(std::filesystem::path(shaderLocation));
    }

    template<class Archive>
    void serialize(Archive& archive,
        RelationshipComponent& relationship) {

        archive(cereal::make_nvp("parentUID", relationship.parent_UID));
        archive(cereal::make_nvp("childrensUIDs", relationship.children));

    }

    template<class Archive>
    void serialize(Archive& archive,
        TransformComponent& transform) {

        archive(cereal::make_nvp("position", transform.position));
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

        RelationshipComponent& relation = m_entity.getComponent<RelationshipComponent>();
        archive(cereal::make_nvp("relationship", relation));

        bool hasMeshRenderComponent = m_entity.hasComponent<MeshRenderComponent>();
        archive(cereal::make_nvp("hasMeshRenderComponent", hasMeshRenderComponent));
        if (hasMeshRenderComponent) {
            MeshRenderComponent& meshRender = m_entity.getComponent<MeshRenderComponent>();
            archive(cereal::make_nvp("meshRenderComponent", meshRender));
        }
    }

    template<class Archive>
    void load(Archive& archive,
        Entity& m_entity) {

        uid id;
        std::string name;
        archive(cereal::make_nvp("id", id));
        archive(cereal::make_nvp("name", name));

        m_entity.addComponent<TagComponent>() = TagComponent(name, id);

        DEER_CORE_INFO("id : {0}", id);
        DEER_CORE_INFO("name : {0}", name);

        m_entity.addComponent<TransformComponent>();
        archive(cereal::make_nvp("transform", m_entity.getComponent<TransformComponent>()));

        m_entity.addComponent<RelationshipComponent>();
        archive(cereal::make_nvp("relationship", m_entity.getComponent<RelationshipComponent>()));

        bool hasMeshRenderComponent;
        archive(cereal::make_nvp("hasMeshRenderComponent", hasMeshRenderComponent));
        if (hasMeshRenderComponent) {
            m_entity.addComponent<MeshRenderComponent>();
            archive(cereal::make_nvp("meshRenderComponent", m_entity.getComponent<MeshRenderComponent>()));
        }

        m_entity.update();
    }

    template<class Archive>
    void save(Archive& archive,
        Ref<Environment> const& m_environment) {
        std::vector<Entity> entityMap;

        auto view = m_environment->m_registry.view<TagComponent>();

        for (auto entity : view) {
            TagComponent& tag = view.get<TagComponent>(entity);
            entityMap.push_back(m_environment->getEntity(tag.entityUID));
        }

        archive(cereal::make_size_tag(static_cast<cereal::size_type>(entityMap.size()))); // number of elements
        for (auto&& v : entityMap)
            archive(v);
    }

    template<class Archive>
    void load(Archive& archive,
        Ref<Environment> & m_environment) {
        std::vector<Entity> entityMap;
        //archive(cereal::make_nvp("entities", entityMap));

        
        cereal::size_type size;
        archive(cereal::make_size_tag(size));

        entityMap.resize(static_cast<std::size_t>(size));
        for (auto& v : entityMap) {
            v = m_environment->createEmptyEntity();
            archive(v);
        }
    }

    template<class Archive>
    void serialize(Archive& archive,
        Ref<Scene> & m_scene) {

        archive(cereal::make_nvp("main_environment", m_scene->getMainEnviroment()));
    }
}