#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Enviroment.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Asset/AssetManager.h"
#include "cereal/cereal.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/string.hpp"
#include "entt/entt.hpp"
#include "SerializationRender.h"

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

// Intermediate structs
namespace Deer {
    struct EntityVector_Environment {
        std::vector<Entity> entities;
        const Ref<Environment>& environment;
        EntityVector_Environment(const Ref<Environment>& _environment)
            : environment(_environment) { }
    };
}

namespace Deer {
    // SCRIPT COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        ScriptComponent& scriptComponent) {

        archive(cereal::make_nvp("scriptID", scriptComponent.scriptID));
    }

    // CAMERA COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        CameraComponent& camera) {

        archive(cereal::make_nvp("aspect", camera.aspect));
        archive(cereal::make_nvp("fov", camera.fov));
        archive(cereal::make_nvp("farZ", camera.farZ));
        archive(cereal::make_nvp("nearZ", camera.nearZ));
    }

    // RELATIONSHIP COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        RelationshipComponent& relationship) {

        archive(cereal::make_nvp("parentUID", relationship.parent_UID));
        archive(cereal::make_nvp("childrensUIDs", relationship.children));

    }

    // TRANSFORM COMPONENT
    template<class Archive>
    void serialize(Archive& archive,
        TransformComponent& transform) {

        archive(cereal::make_nvp("position", transform.position));
        archive(cereal::make_nvp("scale", transform.scale));
        archive(cereal::make_nvp("rotation", transform.rotation));
        
    }

    // ENTITY
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

        bool hasCameraComponent = m_entity.hasComponent<CameraComponent>();
        archive(cereal::make_nvp("hasCameraComponent", hasCameraComponent));
        if (hasCameraComponent) {
            CameraComponent& camera = m_entity.getComponent<CameraComponent>();
            archive(cereal::make_nvp("cameraComponent", camera));
        }

        bool hasTextureBindingComponent = m_entity.hasComponent<TextureBindingComponent>();
        archive(cereal::make_nvp("hasTextureBindingComponent", hasTextureBindingComponent));
        if (hasTextureBindingComponent) {
            TextureBindingComponent& textureBinding = m_entity.getComponent<TextureBindingComponent>();
            archive(cereal::make_nvp("textureBindingComponent", textureBinding));
        }

        bool hasScriptComponent = m_entity.hasComponent<ScriptComponent>();
        archive(cereal::make_nvp("hasScriptComponent", hasScriptComponent));
        if (hasScriptComponent) {
            ScriptComponent& scriptComponent = m_entity.getComponent<ScriptComponent>();
            archive(cereal::make_nvp("scriptComponent", scriptComponent));
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

        m_entity.addComponent<TransformComponent>();
        archive(cereal::make_nvp("transform", m_entity.getComponent<TransformComponent>()));

        m_entity.addComponent<RelationshipComponent>();
        archive(cereal::make_nvp("relationship", m_entity.getComponent<RelationshipComponent>()));

        bool hasMeshRenderComponent;
        archive(cereal::make_nvp("hasMeshRenderComponent", hasMeshRenderComponent));
        if (hasMeshRenderComponent) {
            MeshRenderComponent& meshRender = m_entity.addComponent<MeshRenderComponent>();
            archive(cereal::make_nvp("meshRenderComponent", meshRender));
        }

        bool hasCameraComponent;
        archive(cereal::make_nvp("hasCameraComponent", hasCameraComponent));
        if (hasCameraComponent) {
            CameraComponent& camera = m_entity.addComponent<CameraComponent>();
            archive(cereal::make_nvp("cameraComponent", camera));
        }

        bool hasTextureBindingComponent;
        archive(cereal::make_nvp("hasTextureBindingComponent", hasTextureBindingComponent));
        if (hasTextureBindingComponent) {
            TextureBindingComponent& textureBinding = m_entity.addComponent<TextureBindingComponent>();
            archive(cereal::make_nvp("textureBindingComponent", textureBinding));
        }

        bool hasScriptComponent;
        archive(cereal::make_nvp("hasScriptComponent", hasScriptComponent));
        if (hasScriptComponent) {
            ScriptComponent& scriptComponent = m_entity.addComponent<ScriptComponent>();
            archive(cereal::make_nvp("scriptComponent", scriptComponent));
        }

        m_entity.updateExecution();
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
        Ref<Environment> & m_environment) {
        EntityVector_Environment entityMap(m_environment);

        uid mainCameraUID;

        archive(cereal::make_nvp("entities", entityMap));
        archive(cereal::make_nvp("mainCameraUID", mainCameraUID));

        if (mainCameraUID != 0)
            m_environment->setMainCamera(m_environment->getEntity(mainCameraUID));

    }

    template<class Archive>
    void serialize(Archive& archive,
        Ref<Scene> & m_scene) {

        archive(cereal::make_nvp("main_environment", m_scene->getMainEnviroment()));
    }
}