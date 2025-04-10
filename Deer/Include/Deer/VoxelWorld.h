#pragma once
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

	struct VoxelWorldProps {
		uint8_t chunkSizeX = 4;
		uint8_t chunkSizeY = 2;
		uint8_t chunkSizeZ = 4;

		VoxelWorldProps() = default;
		VoxelWorldProps(uint8_t _chunkSizeX, 
			uint8_t _chunkSizeY, 
			uint8_t _chunkSizeZ) : 
				chunkSizeX(_chunkSizeX), 
				chunkSizeY(_chunkSizeY), 
				chunkSizeZ(_chunkSizeZ) { }

		inline int getChunkCount() const { return chunkSizeX * chunkSizeY * chunkSizeZ; }
		inline int getLayerCount() const { return chunkSizeX * chunkSizeZ; }
		inline int getWorldChunkID(ChunkID chunkID) const { return chunkID.z + chunkID.y * chunkSizeZ + chunkID.x * chunkSizeZ * chunkSizeY; }

		inline int getWorldLayerID(LayerID layerID) const { return layerID.z + layerID.x * chunkSizeZ; }

		inline LayerID getLayerID(int id) const {
			LayerID l_id;

			l_id.x = id / chunkSizeZ;
			id -= l_id.x * chunkSizeZ;

			l_id.z = id;
			return l_id;
		}

		inline ChunkID getChunkID(int id) const {
			ChunkID c_id;
			
			c_id.x = id / (chunkSizeZ * chunkSizeY);
			id -= c_id.x * (chunkSizeZ * chunkSizeY);

			c_id.y = id / chunkSizeZ;
			id -= c_id.y * chunkSizeZ;

			c_id.z = id;
			return c_id;
		}

		inline bool isValid(ChunkID chunkID) const {
			return chunkID.x >= 0 && chunkID.x < chunkSizeX
				&& chunkID.y >= 0 && chunkID.y < chunkSizeY
				&& chunkID.z >= 0 && chunkID.z < chunkSizeZ;
		}

		inline bool isValid(LayerID layerID) const {
			return layerID.x >= 0 && layerID.x < chunkSizeX
				&& layerID.z >= 0 && layerID.z < chunkSizeZ;
		}

		inline int getMaxVoxelCount() {
			return getChunkCount() * CHUNK_VOXELS;
		}

		inline void clampCordinates(VoxelCordinates& coords) const {
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

		inline void clampAndSetMinMax(VoxelCordinates& min, VoxelCordinates& max) const {
			VoxelCordinates a_cache = min;
			VoxelCordinates b_cache = max;

			for (int x = 0; x < 3; x++){
				if (a_cache[x] > b_cache[x]){
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

	class VoxelWorld {
		public:
			VoxelWorld(const VoxelWorldProps& props);
			VoxelWorld(const VoxelWorld&) = delete;
			VoxelWorld& operator=(VoxelWorld&) = delete;
	
			// Voxel data
			Voxel readVoxel(VoxelCordinates);
			void setVoxel(VoxelCordinates, Voxel value);

			// Advanced manipulations
			void fillVoxels(VoxelCordinates min, VoxelCordinates max, Voxel value);
			void remplaceVoxels(VoxelCordinates min, VoxelCordinates max, Voxel ref, Voxel value);
	
			// Layer data
			LayerVoxel readLayerVoxel(int x, int z);
			uint16_t calculateLayerVoxelHeight(int x, int z);
	
			// Math operations
			VoxelRayResult rayCast(glm::vec3 position, glm::vec3 dir, float maxDistance = 10.0f);
			VoxelRayResult rayCast_editor(glm::vec3 position, glm::vec3 dir, float maxDistance = 10.0f);
	
			inline const VoxelWorldProps& getVoxelWorldProps() const { return m_worldProps; }
		private:
			VoxelWorldProps m_worldProps;
	
			// Chunk data
			Scope<Chunk[]> m_chunks;
			Scope<Layer[]> m_layers;
	
			LayerVoxel& modLayerVoxel(int x, int z);
	#ifdef DEER_RENDER
		public:
			void render(const SceneCamera& camera);
	
			// Chunk baking
			void bakeNextChunk();
	
			// Light baking
			void bakeAmbientLight(int minX, int maxX, int minZ, int maxZ);
			void bakeAmbientLightFromPoint(int x, int z);
	
			void bakeVoxelLight(int minX, int maxX, int minY, int maxY,int minZ, int maxZ);
			void bakeVoxelLightFromPoint(int x, int y, int z);
	
			// Light data
			VoxelLight readLight(VoxelCordinates);
			VoxelLight& modLight(VoxelCordinates);
		private:
			Scope<VoxelWorldRenderData> m_renderData;
	
			// Chunk vertex creation
			void genSolidVoxel(ChunkID chunkID, ChunkVoxelID chunkVoxelID);
	
			//Light propagation
			void resolveNextAmbientLightPropagation();
			void resolveNextVoxelLightPropagation();
	#endif
		};
	
}

