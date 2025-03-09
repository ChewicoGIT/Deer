#pragma once
#include "Deer/Voxels/VoxelInfo.h"
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
        case VoxelInfoType::Solid :
            blockTypeChar = VOXEL_INFO_TYPE_SOLID;
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
        else if (blockTypeString == VOXEL_INFO_TYPE_SOLID)
            block.type = VoxelInfoType::Solid;
        else {
            block.type = VoxelInfoType::Air;
            DEER_CORE_ERROR("Failed to resolve voxel type for {0}, unknown type : {1}",
                block.name.c_str(), blockTypeString.c_str());
        }
    } 
}