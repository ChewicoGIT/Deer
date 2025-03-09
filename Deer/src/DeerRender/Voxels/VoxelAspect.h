#pragma once
#include "Deer/Core/Core.h"

#define VOXEL_ASPECT_TYPE_EMPTY "empty"
#define VOXEL_ASPECT_TYPE_VOXEL "voxel"
#define VOXEL_ASPECT_TYPE_CUSTOM "custom"

// TEMP
#define VOXEL_TEXTURE_SIZE_X 128
#define VOXEL_TEXTURE_SIZE_Y 128

namespace Deer {
    enum class VoxelAspectType : uint8_t {
        Undefined = 0,
        Empty = 1,
        Voxel = 2,
        Custom = 3
    };

    struct VoxelTextureFaceDefinition {
        std::string textureFaces[6];

        inline std::string& operator[](size_t index) {
            return textureFaces[index];
        }
    };

    struct VoxelAspectDefinition {
        std::string voxelName;
        VoxelTextureFaceDefinition textureFaces;
        VoxelAspectType aspectType  = VoxelAspectType::Undefined;

        VoxelAspectDefinition() = default;
        VoxelAspectDefinition(VoxelAspectType _aspectType) : aspectType(_aspectType) { }
    };

    struct VoxelAspect {
        VoxelAspectDefinition definition;
        uint16_t textureFacesIDs[6]{};
    };
}