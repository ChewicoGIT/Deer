// Structure definition for voxel and voxel manipulation
// copyright Copyright (c) 2025 Deer
#pragma once
#include <stdint.h>

#include <array>
#include <string>
#include <vector>

#include "Deer/Memory.h"

#ifdef DEER_RENDER
#include "DeerRender/VoxelAspect.h"

namespace Deer {
	class Texture2D;
	class Shader;
}  // namespace Deer
#endif

#define VOXEL_INFO_TYPE_AIR "air"
#define VOXEL_INFO_TYPE_VOXEL "voxel"
#define VOXEL_INFO_TYPE_TRANSPARENT_VOXEL "transparentVoxel"
#define VOXEL_INFO_TYPE_CUSTOM "custom"

#define CHUNK_SIZE_X 32
#define CHUNK_SIZE_Y 32
#define CHUNK_SIZE_Z 32
#define CHUNK_SIZE(axis) \
	((axis == 0) ? CHUNK_SIZE_X : (axis == 1) ? CHUNK_SIZE_Y : CHUNK_SIZE_Z)

#define LAYER_VOXELS CHUNK_SIZE_X* CHUNK_SIZE_Z
#define CHUNK_VOXELS CHUNK_SIZE_X* CHUNK_SIZE_Y* CHUNK_SIZE_Z

// TODO: Change this to be a inline function
#define VOXEL_POSITION(id) \
	id.z + id.y* CHUNK_SIZE_Z + id.x* CHUNK_SIZE_Z* CHUNK_SIZE_Y
#define LAYER_VOXEL_POSITION(id) id.z + id.x* CHUNK_SIZE_Z

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

// TODO: Change this to be a inline function
#define NORMAL_DIR(axis, normal) normalDirs[axis + normal * 3]

namespace Deer {
	struct Voxel;
	struct LayerVoxel;

	extern Voxel nullVoxel;
	extern Voxel emptyVoxel;
	extern LayerVoxel nullLayerVoxel;
	extern int normalDirs[3 * 6];

	enum NormalDirection : uint8_t {
		NORMAL_LEFT = 0,
		NORMAL_RIGHT = 1,
		NORMAL_DOWN = 2,
		NORMAL_UP = 3,
		NORMAL_BACK = 4,
		NORMAL_FRONT = 5
	};

	enum class VoxelInfoType : uint8_t {
		Air = 0,
		Voxel = 1,
		TransparentVoxel = 2,
		Custom = 3
	};

	// Defines the general data of a voxel id stored in the array
	// VoxelData::voxelsInfo
	struct VoxelInfo {
		std::string name;
		VoxelInfoType type = VoxelInfoType::Air;
	};

	// Namespace to load and manage voxel data
	namespace VoxelData {
		// List of the voxels loaded with loadVoxelsData()
		extern std::vector<VoxelInfo> voxelsInfo;

		// Loads basic voxel data from folder DEER_VOXEL_DATA_PATH defined in
		// DataStore.h
		void loadVoxelsData();
		void createExampleVoxelData();

		int32_t getVoxelID(const std::string&);

#ifdef DEER_RENDER
		// List of the voxels Aspect loaded with loadVoxelsAspect()
		extern std::vector<VoxelAspect> voxelsAspect;

		// Loads voxel aspect from folder DEER_VOXEL_ASPECT_PATH defined in
		// DataStore.h
		void loadVoxelsAspect();
		void createExampleVoxelAspect();

		// Generates the texture atlas that the voxels demanded from folder
		// DEER_VOXEL_TEXTURE_PATH defined in DataStore.h Warning : This
		// function must be called with a render context, otherwise this will
		// crash
		void generateTextureAtlas();
		// Loads the shaders for rendering chunks from folder
		// DEER_VOXEL_SHADER_PATH defined in DataStore.h
		void loadVoxelsShaders();

		// Returns with & height of the texture atlas generated
		// Warning: If you call this before generate Texture Atlas the return
		// value will be 0
		int getVoxelTextureAtlasSize();
		// Texture atlas created with generateTextureAtlas() call
		// Warning: You must have called generateTextureAtlas() in order to work
		Ref<Texture2D>& getVoxelColorTextureAtlas();
		// Returns the shader created with loadVoxelsShaders()
		// Warning: You must have called loadVoxelsShaders() in order to work
		Ref<Shader>& getSolidVoxelShader();

#endif
	}  // namespace VoxelData

	// Structure to define what a voxel inside a world must have
	struct Voxel {
		// Reference to the voxel id
		uint16_t id = 0;

		Voxel() = default;
		Voxel(uint16_t _id) : id(_id) {}

		inline bool operator==(const Voxel& b) const { return id == b.id; }
		inline bool isVoxelType() const {
			return VoxelData::voxelsInfo[id].type == VoxelInfoType::Voxel;
		}
	};

	// Structure to define the general cordinates of a voxel in the world
	struct VoxelCordinates {
		union {
			struct {
				int32_t x, y, z;
			};
			std::array<int32_t, 3> data;
		};

		VoxelCordinates(int32_t _x = 0, int32_t _y = 0, int32_t _z = 0)
		    : x(_x), y(_y), z(_z) {}

		inline int32_t& operator[](int id) { return data[id]; }
		inline bool operator==(const VoxelCordinates& b) const {
			return x == b.x && y == b.y && z == b.z;
		}
		inline bool isNull() const { return x < 0 || y < 0 || z < 0; }
		inline void makeNull() { x = -1; }
	};

	// Stucture that defines the info of a layer voxel
	struct LayerVoxel {
		uint16_t height = 0;
#ifdef DEER_RENDER
		uint16_t ambient_light_height = 0;
#endif

		LayerVoxel() = default;
		LayerVoxel(uint16_t _height) : height(_height) {}
	};

	// Returning info of a raycast
	struct VoxelRayResult {
		float distance = 0;
		VoxelCordinates hitPos;
		uint8_t face = 0;
	};

	// Coordinates of a chunk
	struct ChunkID {
		union {
			struct {
				uint16_t x;
				uint16_t y;
				uint16_t z;
			};
			std::array<uint16_t, 3> axis;
		};

		ChunkID(uint16_t _x = 0, uint16_t _y = 0, uint16_t _z = 0)
		    : x(_x), y(_y), z(_z) {}

		inline bool operator==(const ChunkID& b) const {
			return x == b.x && y == b.y && z == b.z;
		}
		inline uint16_t& operator[](size_t i) { return axis[i]; }
	};

	struct ChunkIDHash {
		size_t operator()(const ChunkID& chunk) const {
			size_t h1 = std::hash<uint16_t>{}(chunk.x);
			size_t h2 = std::hash<uint16_t>{}(chunk.y);
			size_t h3 = std::hash<uint16_t>{}(chunk.z);

			size_t result = h1;
			result = result * 31 + h2;
			result = result * 31 + h3;
			return result;
		}
	};

	// Cordinates of a Layer
	struct LayerID {
		uint16_t x = 0;
		uint16_t z = 0;

		LayerID() = default;
		LayerID(uint16_t _x, uint16_t _z) : x(_x), z(_z) {}
		inline bool operator==(const LayerID& b) const {
			return x == b.x && z == b.z;
		}
	};

	// Coordinates of a layer voxel relative to the Layer Chunk
	struct LayerVoxelID {
		uint8_t x = 0;
		uint8_t z = 0;

		LayerVoxelID() = default;
		LayerVoxelID(uint8_t _x, uint8_t _z = 0) : x(_x), z(_z) {}
	};

	// Coordinates of a voxel inside a Chunk
	struct ChunkVoxelID {
		union {
			struct {
				uint8_t x;
				uint8_t y;
				uint8_t z;
			};
			std::array<uint8_t, 3> axis;
		};

		ChunkVoxelID(uint8_t _x = 0, uint8_t _y = 0, uint8_t _z = 0)
		    : x(_x), y(_y), z(_z) {}
		inline uint8_t& operator[](size_t i) { return axis[i]; }
	};

	// Extracts the chunk coordinaes and the chunk voxel coordinates from a
	// world position
	inline void extractChunkCordinates(uint32_t x, uint32_t y, uint32_t z,
	                                   ChunkID& _chunkID,
	                                   ChunkVoxelID& _chunkVoxelID) {
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

	// Extracts the chunk coordinaes and the chunk voxel chunk coordinates from
	// a world position
	inline void extractChunkCordinates(VoxelCordinates coords,
	                                   ChunkID& _chunkID,
	                                   ChunkVoxelID& _chunkVoxelID) {
		uint16_t posX = coords.x;
		uint16_t posY = coords.y;
		uint16_t posZ = coords.z;

		_chunkID.x = posX >> 5;
		_chunkID.y = posY >> 5;
		_chunkID.z = posZ >> 5;

		_chunkVoxelID.x = posX & 31;
		_chunkVoxelID.y = posY & 31;
		_chunkVoxelID.z = posZ & 31;
	}

	// Extracts the layer chunk coordinaes and the layer chunk voxel coordinates
	// from a world position
	inline void extractLayerCordinates(uint32_t x, uint32_t z,
	                                   LayerID& _layerID,
	                                   LayerVoxelID& _layerVoxelID) {
		uint16_t posX = x;
		uint16_t posZ = z;

		_layerID.x = posX >> 5;
		_layerID.z = posZ >> 5;

		_layerVoxelID.x = posX & 31;
		_layerVoxelID.z = posZ & 31;
	}
}  // namespace Deer
