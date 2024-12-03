#pragma once
#include <cstdint>

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32

#define CHUNK_VOXELS CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z
#define VOXEL_POSITION(id) id.z + id.y * CHUNK_SIZE_Z + id.x * CHUNK_SIZE_Z * CHUNK_SIZE_Y

#define INTERNAL_X_POS 0
#define INTERNAL_Y_POS 1
#define INTERNAL_Z_POS 2

#define NORMAL_LEFT 0
#define NORMAL_RIGHT 1
#define NORMAL_DOWN 2
#define NORMAL_UP 3
#define NORMAL_BACK 4
#define NORMAL_FRONT 5
#define NORMAL_DIR(axis, normal) normalDirs[axis + normal * 3]
#define NORMAL_VERTEX_POS(axis, id, normal) normalFacePositions[axis + id * 3 + normal * 3 * 4]
#define NORMAL_VERTEX_POS_DENORMALIZED(axis, id, normal) normalFacePositionsDenormalized[axis + id * 3 + normal * 3 * 4]
#define VERTEX_UV(axis, id) uvFace[axis + id * 2]

namespace Deer {
	struct Voxel;

	extern Voxel nullVoxel;
	extern Voxel emptyVoxel;
	extern int normalDirs[3 * 6];
	extern int normalFacePositions[3 * 4 * 6];
	extern int normalFacePositionsDenormalized[3 * 4 * 6];
	extern int uvFace[2 * 4];

	struct Voxel {
		uint16_t id;

		Voxel() : id(0) { }
		Voxel(uint16_t _id) : id(_id) { }
	};

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

		ChunkVoxelID() : x(0), y(0), z(0) { }
		ChunkVoxelID(uint8_t _x, uint8_t _y = 0, uint8_t _z = 0) : x(_x), y(_y), z(_z) { }
	};

	inline void extractCordinates(int x, int y, int z, ChunkID& _chunkID, ChunkVoxelID& _chunkVoxelID) {
		uint16_t posX = x;
		uint16_t posY = y;
		uint16_t posZ = z;

		_chunkID.x = posX >> 5;
		_chunkID.y = posY >> 5;
		_chunkID.z = posZ >> 5;

		_chunkVoxelID.x = posX & 31;
		_chunkVoxelID.y = posY & 31;
		_chunkVoxelID.z = posZ & 31;
	}
}
