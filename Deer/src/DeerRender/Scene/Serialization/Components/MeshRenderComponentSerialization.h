#pragma once
#include "Deer/Scene/Components.h"

namespace Deer{
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
}