#pragma once
#include "Deer/Memory.h"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/VoxelAspect.h"

namespace Deer { class Texture2D; class Shader; }
#endif

#include <vector>
#include <string>
#include <stdint.h>

namespace Deer {
	enum class VoxelInfoType : uint8_t {
		Air = 0,
		Voxel = 1,
		TransparentVoxel = 2,
		Custom = 3
	};

	struct VoxelInfo {
		std::string name;
		VoxelInfoType type = VoxelInfoType::Air;
	};

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