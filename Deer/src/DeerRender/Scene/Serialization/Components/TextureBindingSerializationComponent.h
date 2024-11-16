#pragma once
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Texture.h"
#include <string>
#include <vector>

namespace Deer {
    struct TextureBinding {
        std::string texturePath;
        unsigned char bindingID;

        TextureBinding() = default;
        TextureBinding(std::string _texturePath, unsigned char _bindingID) : texturePath(_texturePath), bindingID(_bindingID) { }
    };

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
}