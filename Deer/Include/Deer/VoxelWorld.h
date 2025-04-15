// copyright Copyright (c) 2025 Deer
#pragma once
#include <array>

#include "Deer/Memory.h"
#include "Deer/Voxel.h"

#ifdef DEER_RENDER
#include "DeerRender/LightVoxel.h"
#endif

#include "glm/glm.hpp"

namespace Deer {
	class Chunk;
	class Layer;
	struct SceneCamera;
	struct VoxelWorldProps;
	struct VoxelWorldRenderData;

	// Properties of a Voxel World
	struct VoxelWorldProps {
		union {
			struct {
				uint8_t chunkSizeX;
				uint8_t chunkSizeY;
				uint8_t chunkSizeZ;
			};
			std::array<uint8_t, 3> axis;
		};

		VoxelWorldProps() = default;
		VoxelWorldProps(uint8_t _chunkSizeX, uint8_t _chunkSizeY,
		                uint8_t _chunkSizeZ)
		    : chunkSizeX(_chunkSizeX),
		      chunkSizeY(_chunkSizeY),
		      chunkSizeZ(_chunkSizeZ) {}

		inline uint8_t &operator[](size_t i) { return axis[i]; }

		// Returns the count of chunks
		inline int getChunkCount() const {
			return chunkSizeX * chunkSizeY * chunkSizeZ;
		}
		// Returns the count of layers
		inline int getLayerCount() const { return chunkSizeX * chunkSizeZ; }
		// Returns the internal id of a chunk relative to a Voxel World Props
		// from a chunk id
		inline int getWorldChunkID(ChunkID chunkID) const {
			return chunkID.z + chunkID.y * chunkSizeZ +
			       chunkID.x * chunkSizeZ * chunkSizeY;
		}
		// Returns the internal id of a layer relative to a Voxel World Props
		// from a Layer id
		inline int getWorldLayerID(LayerID layerID) const {
			return layerID.z + layerID.x * chunkSizeZ;
		}

		// Extracts the LayerID from a internal Layer id relative to Voxel World
		// Props
		inline LayerID getLayerID(int id) const {
			LayerID l_id;

			l_id.x = id / chunkSizeZ;
			id -= l_id.x * chunkSizeZ;

			l_id.z = id;
			return l_id;
		}
		// Extracts the ChunkID from a internal Chunk id relative to Voxel World
		// Props
		inline ChunkID getChunkID(int id) const {
			ChunkID c_id;

			c_id.x = id / (chunkSizeZ * chunkSizeY);
			id -= c_id.x * (chunkSizeZ * chunkSizeY);

			c_id.y = id / chunkSizeZ;
			id -= c_id.y * chunkSizeZ;

			c_id.z = id;
			return c_id;
		}

		// Checks if the Chunk id is inside the voxel World bounds
		inline bool isValid(ChunkID chunkID) const {
			return chunkID.x >= 0 && chunkID.x < chunkSizeX && chunkID.y >= 0 &&
			       chunkID.y < chunkSizeY && chunkID.z >= 0 &&
			       chunkID.z < chunkSizeZ;
		}

		// Checks if the Layer id is inside the voxel World bounds
		inline bool isValid(LayerID layerID) const {
			return layerID.x >= 0 && layerID.x < chunkSizeX && layerID.z >= 0 &&
			       layerID.z < chunkSizeZ;
		}

		// Returns the max amount of voxels in the Voxel World Props
		inline int getMaxVoxelCount() const {
			return getChunkCount() * CHUNK_VOXELS;
		}

		// Clamps the coordinates of a Voxel World Coordinates to be inside the
		// voxel world props
		inline void clampCordinates(VoxelCordinates &coords) const {
			if (coords.x < 0)
				coords.x = 0;
			else if (coords.x >= chunkSizeX * CHUNK_SIZE_X)
				coords.x = chunkSizeX * CHUNK_SIZE_X - 1;

			if (coords.y < 0)
				coords.y = 0;
			else if (coords.y >= chunkSizeY * CHUNK_SIZE_Y)
				coords.y = chunkSizeY * CHUNK_SIZE_Y - 1;

			if (coords.z < 0)
				coords.z = 0;
			else if (coords.z >= chunkSizeZ * CHUNK_SIZE_Z)
				coords.z = chunkSizeZ * CHUNK_SIZE_Z - 1;
		}

		// Takes 2 Voxel coordinates and outputs them in the same variables
		// being the min with the min values and the max with the max This is
		// useful for loops
		inline void clampAndSetMinMax(VoxelCordinates &min,
		                              VoxelCordinates &max) const {
			VoxelCordinates a_cache = min;
			VoxelCordinates b_cache = max;

			for (int x = 0; x < 3; x++) {
				if (a_cache[x] > b_cache[x]) {
					max[x] = a_cache[x];
					min[x] = b_cache[x];
				} else {
					min[x] = a_cache[x];
					max[x] = b_cache[x];
				}
			}

			clampCordinates(min);
			clampCordinates(max);
		}
	};

	// Class to manage the voxels
	class VoxelWorld {
	   public:
		// Warning: Do not change the voxel data content since that could make
		// undefined behaviour
		VoxelWorld(const VoxelWorldProps &props);
		// This class can not be copyed
		// TODO: Make a function to duplicate a voxel World
		VoxelWorld(const VoxelWorld &) = delete;
		VoxelWorld &operator=(VoxelWorld &) = delete;

		// Returns the voxel in a voxel coordinates
		Voxel readVoxel(VoxelCordinates);
		// Sets the voxel in the coordinates to the value
		void setVoxel(VoxelCordinates, Voxel value);

		// Fills a space with the voxel value inside the 2 coordinates
		// Note that you don't have to give then ordeered by min and max
		void fillVoxels(VoxelCordinates, VoxelCordinates, Voxel value);
		// Remplaces the ref voxel with the value of a space inside the 2
		// coordinates
		// Note that you don't have to give then ordeered by min and max
		void remplaceVoxels(VoxelCordinates, VoxelCordinates, Voxel ref,
		                    Voxel value);

		// Returns the layer data of a woorld coordinates
		// Note out of bounds will return a default Layer Voxel
		LayerVoxel readLayerVoxel(int x, int z);
		// Calculates the max height of a layer in a space
		// Note out of bounds will return a 0 of height
		// Tip: this will calculate, you should use the cached height in a layer
		// voxel
		uint16_t calculateLayerVoxelHeight(int x, int z);

		// Raycast a ray from a source and dir
		VoxelRayResult rayCast(glm::vec3 position, glm::vec3 dir,
		                       float maxDistance = 10.0f);
		// Raycast a ray from a source and dir ignoring if the ray stats inside
		// a voxel
		VoxelRayResult rayCast_editor(glm::vec3 position, glm::vec3 dir,
		                              float maxDistance = 10.0f);

		// Returns the voxel world props used in the voxel world
		// Note that you can't change the world size unless you create a new
		// Voxel World
		inline const VoxelWorldProps &getVoxelWorldProps() const {
			return m_worldProps;
		}
#ifdef DEER_RENDER
	   public:
		// Renders the current voxel world with a specified scene camera
		void render(const SceneCamera &);
		// Generates the next chunk mesh
		void bakeNextChunk();

		// Light data
		VoxelLight readLight(VoxelCordinates);
		VoxelLight &modLight(VoxelCordinates);

	   private:
		Scope<VoxelWorldRenderData> m_renderData;

		// Chunk vertex creation
		void genSolidVoxel(ChunkID chunkID, ChunkVoxelID chunkVoxelID);

		// --- Light propagation ---
		// Warning: This function is private and needs to have min and max
		// clamped and in order
		void bakeVoxelLight(VoxelCordinates min, VoxelCordinates max);
		void bakeVoxelLightFromPoint(VoxelCordinates);
		void bakeAmbientLight(int minX, int maxX, int minZ, int maxZ);
		void bakeAmbientLightFromPoint(int x, int z);

		void resolveNextAmbientLightPropagation();
		void resolveNextVoxelLightPropagation();
#endif
	   private:
		VoxelWorldProps m_worldProps;

		Scope<Chunk[]> m_chunks;
		Scope<Layer[]> m_layers;

		LayerVoxel &modLayerVoxel(int x, int z);
	};
}  // namespace Deer
