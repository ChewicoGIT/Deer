/**
 * @file VoxelAspect.h
 * @author chewico@frostdeer.com
 * @brief File to save the voxel aspect data
 * 
 * @copyright Copyright (c) 2025
 */
#pragma once
#include "Deer/Voxel.h"

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

        /**
         * @brief Get the texture id for the voxel face
         * 
         * @param face face of the texture defined in the enum NormalDirection of Voxel.h
         * @return uint16_t texture id in the texture atlas
         */
        inline uint16_t getTextureID(uint8_t face) { return textureFacesIDs[face]; }
    };
}