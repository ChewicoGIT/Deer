#pragma once
#include "VoxelInfo.h"

#include <vector>

#ifdef DEER_RENDER
#include "Deer/Core/Memory.h"
#include "DeerRender/Voxels/VoxelAspect.h"

namespace Deer { class Texture2D; }
#endif

namespace Deer {
    namespace VoxelData {
        extern std::vector<VoxelInfo> voxelsInfo;

        void loadVoxelsData();
        void createExampleVoxelData();

        int32_t getVoxelID(const std::string&);

#ifdef DEER_RENDER
        extern std::vector<VoxelAspect> voxelsAspect;

        void loadVoxelsAspect();
        void createExampleVoxelAspect();
        void generateTextureAtlas();

        int getVoxelTextureAtlasSize();
        Ref<Texture2D>& getVoxelColorTextureAtlas();
#endif
    }
}