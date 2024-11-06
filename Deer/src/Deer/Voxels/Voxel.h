#pragma once
#include <cstdint>

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32

#define CHUNK_VOXELS CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z
#define VOXEL_POSITION(X, Y, Z) Z + Y * CHUNK_SIZE_Z + X * CHUNK_SIZE_Z * CHUNK_SIZE_Y

namespace Deer {
	struct Voxel {
		uint16_t id = 0;
	};
}