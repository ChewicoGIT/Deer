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

    template<class Archive>
    void save(Archive & archive, VoxelAspectDefinition const& voxelAspectDefinition) {
        const char* aspectTypeChar = VOXEL_ASPECT_TYPE_EMPTY;
        switch (voxelAspectDefinition.aspectType) {
            case VoxelAspectType::Empty:
                aspectTypeChar = VOXEL_ASPECT_TYPE_EMPTY;
                break;
            case VoxelAspectType::Voxel :
                aspectTypeChar = VOXEL_ASPECT_TYPE_VOXEL;
                break;
            case VoxelAspectType::Custom :
                aspectTypeChar = VOXEL_ASPECT_TYPE_CUSTOM;
                break;
            case VoxelAspectType::Undefined:
                aspectTypeChar = VOXEL_ASPECT_TYPE_EMPTY;
                break;

        }
        std::string aspectTypeString(aspectTypeChar);

        archive(cereal::make_nvp("name", voxelAspectDefinition.voxelName));
        archive(cereal::make_nvp("aspectType", aspectTypeString));
        archive(cereal::make_nvp("textureFaces", voxelAspectDefinition.textureFaces));
    }

    template<class Archive>
    void load(Archive & archive, VoxelAspectDefinition& voxelAspectDefinition) {
        std::string aspectTypeString;

        archive(cereal::make_nvp("name", voxelAspectDefinition.voxelName));
        archive(cereal::make_nvp("aspectType", aspectTypeString));
        archive(cereal::make_nvp("textureFaces", voxelAspectDefinition.textureFaces));
        
        if (aspectTypeString == VOXEL_ASPECT_TYPE_EMPTY)
            voxelAspectDefinition.aspectType = VoxelAspectType::Empty;
        else if (aspectTypeString == VOXEL_ASPECT_TYPE_VOXEL)
            voxelAspectDefinition.aspectType = VoxelAspectType::Voxel;
        else if (aspectTypeString == VOXEL_ASPECT_TYPE_CUSTOM)
            voxelAspectDefinition.aspectType = VoxelAspectType::Custom;
        else {
            voxelAspectDefinition.aspectType = VoxelAspectType::Empty;
            DEER_CORE_ERROR("Failed to resolve voxel aspect type for {0}, unknown type : {1}",
                voxelAspectDefinition.voxelName.c_str(),
                aspectTypeString.c_str());
        }
    } 
}