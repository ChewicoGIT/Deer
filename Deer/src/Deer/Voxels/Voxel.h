#pragma once
#include "VoxelData.h"
#include <cstdint>

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32

#define LAYER_VOXELS CHUNK_SIZE_X * CHUNK_SIZE_Z
#define CHUNK_VOXELS CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z
#define VOXEL_POSITION(id) id.z + id.y * CHUNK_SIZE_Z + id.x * CHUNK_SIZE_Z * CHUNK_SIZE_Y
#define LAYER_VOXEL_POSITION(id) id.z + id.x * CHUNK_SIZE_Z

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define NORMAL_LEFT 0
#define NORMAL_RIGHT 1
#define NORMAL_DOWN 2
#define NORMAL_UP 3
#define NORMAL_BACK 4
#define NORMAL_FRONT 5
#define NORMAL_DIR(axis, normal) normalDirs[axis + normal * 3]

namespace Deer {
	struct Voxel;
	struct LayerVoxel;

	extern Voxel nullVoxel;
	extern Voxel emptyVoxel;
	extern LayerVoxel nullLayerVoxel;
	extern int normalDirs[3 * 6];

	struct Voxel {
		uint16_t id = 0;

		Voxel() = default;
		Voxel(uint16_t _id) : id(_id) { }

		inline bool operator== (const Voxel& b) const {
			return id == b.id;
		}
	};

	struct VoxelCordinates {
		int32_t x = 0;
		int32_t y = 0;
		int32_t z = 0;

		VoxelCordinates() = default;
		VoxelCordinates(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z) {}
	};

	struct LayerVoxel {
		uint16_t height = 0;
#ifdef DEER_RENDER
		uint16_t ambient_light_height = 0;
#endif 

		LayerVoxel() = default;
		LayerVoxel(uint16_t _height) : height(_height) { }
	};

	struct VoxelRayResult {
		float distance = 0;
		int32_t xPos = 0;
		int32_t yPos = 0;
		int32_t zPos = 0;
		uint8_t face = 0;
	};

	struct ChunkID {
		uint16_t x = 0;
		uint16_t y = 0;
		uint16_t z = 0;

		inline bool operator== (const ChunkID& b) const {
			return x == b.x && y == b.y && z == b.z;
		}
	};

	struct LayerID {
		uint16_t x = 0;
		uint16_t z = 0;

		inline bool operator== (const LayerID& b) const {
			return x == b.x && z == b.z;
		}
	};

	struct ChunkIDHash {
		uint64_t operator()(const ChunkID& id) const noexcept {
			return ((uint64_t)(id.x) << 32) |
				((uint64_t)(id.y) << 16) |
				(uint64_t)(id.z);
		}
	};

	struct LayerVoxelID {
		uint8_t x = 0;
		uint8_t z = 0;

		LayerVoxelID() = default;
		LayerVoxelID(uint8_t _x, uint8_t _z = 0) : x(_x), z(_z) { }
	};

	struct ChunkVoxelID {
		uint8_t x = 0;
		uint8_t y = 0;
		uint8_t z = 0;

		ChunkVoxelID() = default;
		ChunkVoxelID(uint8_t _x, uint8_t _y = 0, uint8_t _z = 0) : x(_x), y(_y), z(_z) { }
	};

	inline void extractChunkCordinates(uint32_t x, uint32_t y, uint32_t z, ChunkID& _chunkID, ChunkVoxelID& _chunkVoxelID) {
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

	inline void extractLayerCordinates(uint32_t x, uint32_t z, LayerID& _layerID, LayerVoxelID& _layerVoxelID) {
		uint16_t posX = x;
		uint16_t posZ = z;

		_layerID.x = posX >> 5;
		_layerID.z = posZ >> 5;

		_layerVoxelID.x = posX & 31;
		_layerVoxelID.z = posZ & 31;
	}
}
