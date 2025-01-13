#pragma once
#include "Deer/Scene/Components.h"

namespace Deer{
    template<class Archive>
    void save(Archive& archive,
        MeshRenderComponent const& meshRender) {

        std::string meshLocation = AssetManager::getAssetLocation(meshRender.meshAssetID).generic_string();
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation = AssetManager::getAssetLocation(meshRender.shaderAssetID).generic_string();
        archive(cereal::make_nvp("shader", shaderLocation));
    }

    template<class Archive>
    void load(Archive& archive,
        MeshRenderComponent& meshRender) {

        std::string meshLocation;
        archive(cereal::make_nvp("mesh", meshLocation));
        std::string shaderLocation;
        archive(cereal::make_nvp("shader", shaderLocation));

        meshRender.meshAssetID = AssetManager::loadAsset<Mesh>(std::filesystem::path(meshLocation));
        meshRender.shaderAssetID = AssetManager::loadAsset<Shader>(std::filesystem::path(shaderLocation));
    }
}