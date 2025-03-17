#pragma once
#include "VoxelInfo.h"

#include <vector>

#ifdef DEER_RENDER
#include "Deer/Core/Memory.h"
#include "DeerRender/Voxels/VoxelAspect.h"

namespace Deer { class Texture2D; class Shader; }
#endif

namespace Deer {
    namespace VoxelData {
        extern std::vector<VoxelInfo> voxelsInfo;

        void loadVoxelsData();
        void createExampleVoxelData();

        int32_t getVoxelID(const std::string&);

#ifdef DEER_RENDER
        void createExampleVoxelAspect();

        void loadVoxelsAspect();
        void generateTextureAtlas();
        void loadVoxelsShaders();

        int getVoxelTextureAtlasSize();
        Ref<Texture2D>& getVoxelColorTextureAtlas();
        Ref<Shader>& getSolidVoxelShader();

        extern std::vector<VoxelAspect> voxelsAspect;
#endif
    }
}