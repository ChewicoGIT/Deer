#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Voxels/Voxel.h"

namespace Deer {
	extern VoxelLight lightVoxel;

	struct VoxelLight {
		uint8_t r_light;
		uint8_t g_light;
		uint8_t b_light;
		uint8_t ambient_light;

		VoxelLight(uint8_t _ambient_light = 255) : r_light(0), g_light(0), b_light(0), ambient_light(_ambient_light) { }
	};
}