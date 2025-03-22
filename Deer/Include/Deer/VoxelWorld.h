#pragma once
#include "Deer/Voxel.h"
#include "Deer/Memory.h"

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
			uint8_t _chunkSizeY = 2, 
			uint8_t _chunkSizeZ = 4)
			: chunkSizeX(_chunkSizeX), 
			chunkSizeY(_chunkSizeY), 
			chunkSizeZ(_chunkSizeZ) { }

		inline int getChunkCount() {
			return chunkSizeX * chunkSizeY * chunkSizeZ;
		}

		inline int getLayerCount() {
			return chunkSizeX * chunkSizeZ;
		}

		inline int getWorldChunkID(ChunkID chunkID) {
			return chunkID.z + chunkID.y * chunkSizeZ + chunkID.x * chunkSizeZ * chunkSizeY;
		}

		inline int getWorldLayerID(LayerID layerID) {
			return layerID.z + layerID.x * chunkSizeZ;
		}

		inline LayerID getLayerID(int id) {
			LayerID l_id;

			l_id.x = id / chunkSizeZ;
			id -= l_id.x * chunkSizeZ;

			l_id.z = id;
			return l_id;
		}

		inline ChunkID getChunkID(int id) {
			ChunkID c_id;
			
			c_id.x = id / (chunkSizeZ * chunkSizeY);
			id -= c_id.x * (chunkSizeZ * chunkSizeY);

			c_id.y = id / chunkSizeZ;
			id -= c_id.y * chunkSizeZ;

			c_id.z = id;
			return c_id;
		}

		inline bool isValid(ChunkID chunkID) {
			return chunkID.x >= 0 && chunkID.x < chunkSizeX
				&& chunkID.y >= 0 && chunkID.y < chunkSizeY
				&& chunkID.z >= 0 && chunkID.z < chunkSizeZ;
		}

		inline bool isValid(LayerID layerID) {
			return layerID.x >= 0 && layerID.x < chunkSizeX
				&& layerID.z >= 0 && layerID.z < chunkSizeZ;
		}

		inline int getMaxVoxelCount() {
			return getChunkCount() * CHUNK_VOXELS;
		}
	};

	class VoxelWorld {
		public:
			VoxelWorld(const VoxelWorldProps& props);
	
			// Voxel data
			Voxel readVoxel(int x, int y, int z);
			void setVoxel(int x, int y, int z, Voxel info);
			void fillVoxels(int minX, int maxX, int minY, int maxY, int minZ, int maxZ, Voxel info);
	
			// Layer data
			LayerVoxel readLayerVoxel(int x, int z);
			uint16_t getLayerVoxelHeight(int x, int z);
	
			// Math operations
			VoxelRayResult rayCast(glm::vec3 position, glm::vec3 dir, float maxDistance = 10.0f);
			VoxelRayResult rayCast_editor(glm::vec3 position, glm::vec3 dir, float maxDistance = 10.0f);
	
			inline const VoxelWorldProps& getVoxelWorldProps() { return m_worldProps; }
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
			VoxelLight readLight(int x, int y, int z);
			VoxelLight& modLight(int x, int y, int z);
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

