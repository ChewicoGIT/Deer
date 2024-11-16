#pragma once
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "Deer/Scene/Serialization/SerializationGlobalVars.h"

namespace Deer {
    template <class Archive, typename T>
    void saveComponent(Archive& archive, const std::string& componentName, Entity const& m_entity) {

        bool hasComponent = m_entity.hasComponent<T>();
        archive(cereal::make_nvp(("has_" + componentName).c_str(), hasComponent));
        if (hasComponent) {
            T& component = m_entity.getComponent<T>();
            archive(cereal::make_nvp(componentName.c_str(), component));
        }
    }

    template <class Archive, typename T>
    void loadComponent(Archive& archive, const std::string& componentName, Entity const& m_entity) {

        bool hasComponent;
        archive(cereal::make_nvp(("has_" + componentName).c_str(), hasComponent));
        if (hasComponent) {
            T& component = m_entity.addComponent<T>();
            archive(cereal::make_nvp(componentName.c_str(), component));
        }
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

#ifdef DEER_RENDER
        if (!is_server_serialization) {
            saveComponent<Archive, MeshRenderComponent>(archive, "meshRenderComponent", m_entity);
            saveComponent<Archive, CameraComponent>(archive, "cameraComponent", m_entity);
            saveComponent<Archive, TextureBindingComponent>(archive, "textureBindingComponent", m_entity);
        }
#endif
        saveComponent<Archive, ScriptComponent>(archive, "scriptComponent", m_entity);
    }

    template<class Archive>
    void load(Archive& archive,
        Entity& m_entity) {

        uid id;
        std::string name;
        archive(cereal::make_nvp("id", id));
        archive(cereal::make_nvp("name", name));
        m_entity.addComponent<TagComponent>() = TagComponent(name, id);
        
        TransformComponent& transform = m_entity.addComponent<TransformComponent>();
        archive(cereal::make_nvp("transform", transform));
        
        RelationshipComponent& relationship = m_entity.addComponent<RelationshipComponent>();
        archive(cereal::make_nvp("relationship", relationship));

#ifdef DEER_RENDER
        if (!is_server_serialization) {
            loadComponent<Archive, MeshRenderComponent>(archive, "meshRenderComponent", m_entity);
            loadComponent<Archive, CameraComponent>(archive, "cameraComponent", m_entity);
            loadComponent<Archive, TextureBindingComponent>(archive, "textureBindingComponent", m_entity);
        }
#endif
        loadComponent<Archive, ScriptComponent>(archive, "scriptComponent", m_entity);

        m_entity.updateExecution();
    }

}