#pragma once
#include <cstdint>

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32

#define CHUNK_VOXELS CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z
#define VOXEL_POSITION(id) id.z + id.y * CHUNK_SIZE_Z + id.x * CHUNK_SIZE_Z * CHUNK_SIZE_Y

namespace Deer {
	struct Voxel {
		uint16_t id;

		Voxel() : id(0) { }
		Voxel(uint16_t _id) : id(_id) { }
	};

	extern Voxel nullVoxel;

	struct ChunkID {
		uint16_t x;
		uint16_t y;
		uint16_t z;

		inline bool operator== (const ChunkID& b) const {
			return x == b.x && y == b.y && z == b.z;
		}
	};

	struct ChunkIDHash {
		uint64_t operator()(const ChunkID& id) const noexcept {
			return ((uint64_t)(id.x) << 32) |
				((uint64_t)(id.y) << 16) |
				(uint64_t)(id.z);
		}
	};


	struct ChunkVoxelID {
		uint8_t x;
		uint8_t y;
		uint8_t z;
	};
}
