#pragma once
#include "Deer/Voxel.h"
#include "Deer/Core/Log.h"

#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"

namespace Deer{
    template<class Archive>
    void save(Archive & archive, VoxelInfo const & block) { 
        archive(cereal::make_nvp("name", block.name));
        
        // To avoid breaking things we set it up to Air
        const char* blockTypeChar = VOXEL_INFO_TYPE_AIR;
        switch (block.type)
        {
        case VoxelInfoType::Air :
            blockTypeChar = VOXEL_INFO_TYPE_AIR;
            break;
        case VoxelInfoType::Voxel :
            blockTypeChar = VOXEL_INFO_TYPE_VOXEL;
            break;
        case VoxelInfoType::TransparentVoxel :
            blockTypeChar = VOXEL_INFO_TYPE_TRANSPARENT_VOXEL;
            break;
        case VoxelInfoType::Custom :
            blockTypeChar = VOXEL_INFO_TYPE_CUSTOM;
            break;
        }

        std::string blockTypeString(blockTypeChar);
        archive(cereal::make_nvp("type", blockTypeString));
    }

    template<class Archive>
    void load(Archive & archive, VoxelInfo & block) {archive(cereal::make_nvp("name", block.name));
        std::string blockTypeString;

        archive(cereal::make_nvp("name", block.name));
        archive(cereal::make_nvp("type", blockTypeString));
        
        if (blockTypeString == VOXEL_INFO_TYPE_AIR)
            block.type = VoxelInfoType::Air;
        else if (blockTypeString == VOXEL_INFO_TYPE_VOXEL)
            block.type = VoxelInfoType::Voxel;
        else if (blockTypeString == VOXEL_INFO_TYPE_TRANSPARENT_VOXEL)
            block.type = VoxelInfoType::TransparentVoxel;
        else if (blockTypeString == VOXEL_INFO_TYPE_CUSTOM)
            block.type = VoxelInfoType::Custom;
        else {
            block.type = VoxelInfoType::Air;
            DEER_CORE_ERROR("Failed to resolve voxel type for {0}, unknown type : {1}",
                block.name.c_str(), blockTypeString.c_str());
        }
    } 
}