#pragma once
#include "DeerRender/Voxels/VoxelAspect.h"
#include "Deer/Core/Log.h"
#include "Deer/Voxels/Voxel.h"

#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"

namespace Deer{
    template <class Archive>
    void serialize(Archive& archive, VoxelTextureFaceDefinition& textureFaceDefinitions) {
        archive(cereal::make_nvp("left", textureFaceDefinitions.textureFaces[NORMAL_LEFT]));
        archive(cereal::make_nvp("right", textureFaceDefinitions.textureFaces[NORMAL_RIGHT]));
        archive(cereal::make_nvp("down", textureFaceDefinitions.textureFaces[NORMAL_DOWN]));
        archive(cereal::make_nvp("up", textureFaceDefinitions.textureFaces[NORMAL_UP]));
        archive(cereal::make_nvp("front", textureFaceDefinitions.textureFaces[NORMAL_FRONT]));
        archive(cereal::make_nvp("back", textureFaceDefinitions.textureFaces[NORMAL_BACK]));
    }

    template <class Archive>
    void serialize(Archive& archive, VoxelColorEmission& voxelEmission) {
        archive(cereal::make_nvp("red", voxelEmission.r_value));
        archive(cereal::make_nvp("green", voxelEmission.g_value));
        archive(cereal::make_nvp("blue", voxelEmission.b_value));
    }

    template<class Archive>
    void serialize(Archive & archive, VoxelAspectDefinition& voxelAspectDefinition) {
        archive(cereal::make_nvp("name", voxelAspectDefinition.voxelName));
        archive(cereal::make_nvp("textureFaces", voxelAspectDefinition.textureFaces));
        archive(cereal::make_nvp("emission", voxelAspectDefinition.colorEmission));
    }
}