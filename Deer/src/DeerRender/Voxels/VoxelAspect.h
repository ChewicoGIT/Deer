#pragma once
#include "Deer/Core/Core.h"

// TEMP
#define VOXEL_TEXTURE_SIZE_X 128
#define VOXEL_TEXTURE_SIZE_Y 128

namespace Deer {
    struct VoxelTextureFaceDefinition {
        std::string textureFaces[6];

        inline std::string& operator[](size_t index) {
            return textureFaces[index];
        }
    };

    struct VoxelColorEmission {
        uint8_t r_value = 0;
        uint8_t g_value = 0;
        uint8_t b_value = 0;
    };

    struct VoxelAspectDefinition {
        std::string voxelName;
        VoxelTextureFaceDefinition textureFaces;
        VoxelColorEmission colorEmission;

        VoxelAspectDefinition() = default;
    };

    struct VoxelAspect {
        VoxelAspectDefinition definition;
        uint16_t textureFacesIDs[6]{};

        inline bool isLightSource() {
            return definition.colorEmission.r_value || definition.colorEmission.g_value || definition.colorEmission.b_value;
        }
    };
}