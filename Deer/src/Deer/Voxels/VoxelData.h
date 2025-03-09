#pragma once
#include "VoxelInfo.h"

#include <vector>

namespace Deer {
    namespace VoxelData {
        extern std::vector<VoxelInfo> voxelsInfo;

        void loadVoxelsData();
        void createExampleVoxelData();

        int32_t getVoxelID(const std::string&);

#ifdef DEER_RENDER
        void loadVoxelsAspect();
        void createExampleVoxelAspect();
        void generateTextureAtlas();
#endif
    }
}