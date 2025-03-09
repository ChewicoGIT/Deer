#pragma once
#include "VoxelInfo.h"

#include <vector>

#ifdef DEER_RENDER
#include "Deer/Core/Memory.h"

namespace Deer { class Texture2D; }
#endif

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

        Ref<Texture2D>& getVoxelColorTextureAtlas();
#endif
    }
}