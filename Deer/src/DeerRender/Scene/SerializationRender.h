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
#include <vector>

#ifndef DEER_SERVICE
#include "DeerRender/Render/Texture.h"
#endif

namespace Deer {
    struct TextureBinding {
        std::string texturePath;
        unsigned char bindingID;

        TextureBinding() = default;
        TextureBinding(std::string _texturePath, unsigned char _bindingID) : texturePath(_texturePath), bindingID(_bindingID) { }
    };


#ifndef DEER_SERVICE
    template<class Archive>
    void serialize(Archive& archive,
        TextureBinding& textureBinding) {

        archive(cereal::make_nvp("texturePath", textureBinding.texturePath));
        archive(cereal::make_nvp("bindingID", textureBinding.bindingID));
    }

    template<class Archive>
    void load(Archive& archive,
        TextureBindingComponent& textureBinding) {
        std::vector<TextureBinding> bindings;

        archive(cereal::make_nvp("bindings", bindings));

        int id = 0;
        for (auto& binding : bindings) {
            if (id >= MAX_TEXTURE_BINDINGS)
                break;

            textureBinding.textureAssetID[id] = Project::m_assetManager->loadAsset<Texture2D>(
                std::filesystem::path(binding.texturePath));
            textureBinding.textureBindID[id] = binding.bindingID;
        }
    }

    template<class Archive>
    void save(Archive& archive,
        TextureBindingComponent const& textureBinding) {
        std::vector<TextureBinding> bindings;

        for (int x = 0; x < MAX_TEXTURE_BINDINGS; x++) {
            if (textureBinding.textureAssetID[x] != 0) {
                bindings.push_back(TextureBinding(
                    Project::m_assetManager->getAssetLocation(textureBinding.textureAssetID[x]).generic_string(),
                    textureBinding.textureBindID[x]
                ));
            }
        }

        std::sort(bindings.begin(), bindings.end(), [](TextureBinding& a, TextureBinding& b) {
            return a.bindingID < b.bindingID; });

        archive(cereal::make_nvp("bindings", bindings));
    }

    template<class Archive>
    void save(Archive& archive,
        MeshRenderComponent const& meshRender) {

        std::string meshLocation = Project::m_assetManager->getAssetLocation(meshRender.meshAssetID).generic_string();
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation = Project::m_assetManager->getAssetLocation(meshRender.shaderAssetID).generic_string();
        archive(cereal::make_nvp("shader", shaderLocation));
    }

    template<class Archive>
    void load(Archive& archive,
        MeshRenderComponent& meshRender) {

        std::string meshLocation;
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation;
        archive(cereal::make_nvp("shader", shaderLocation));

        meshRender.meshAssetID = Project::m_assetManager->loadAsset<Mesh>(std::filesystem::path(meshLocation));
        meshRender.shaderAssetID = Project::m_assetManager->loadAsset<Shader>(std::filesystem::path(shaderLocation));
    }
#else

    template<class Archive>
    void serialize(Archive& archive,
        TextureBinding& textureBinding) {

        archive(cereal::make_nvp("texturePath", textureBinding.texturePath));
        archive(cereal::make_nvp("bindingID", textureBinding.bindingID));
    }

    template<class Archive>
    void save(Archive& archive,
        TextureBindingComponent const& textureBinding) {
        std::vector<TextureBinding> bindings;

        std::sort(bindings.begin(), bindings.end(), [](TextureBinding& a, TextureBinding& b) {
            return a.bindingID < b.bindingID; });

        archive(cereal::make_nvp("bindings", bindings));
    }

    template<class Archive>
    void load(Archive& archive,
        TextureBindingComponent& textureBinding) {
        std::vector<TextureBinding> bindings;

        archive(cereal::make_nvp("bindings", bindings));

    }

    template<class Archive>
    void save(Archive& archive,
        MeshRenderComponent const& meshRender) {

        std::string emptyString = "null";
        archive(cereal::make_nvp("mesh", emptyString));
        archive(cereal::make_nvp("shader", emptyString));
    }

    template<class Archive>
    void load(Archive& archive,
        MeshRenderComponent& meshRender) {

        std::string meshLocation;
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation;
        archive(cereal::make_nvp("shader", shaderLocation));
    }

#endif
}