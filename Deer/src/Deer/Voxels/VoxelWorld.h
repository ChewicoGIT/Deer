#pragma once

#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/VoxelWorldProps.h"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/ChunkUpdateQueue.h"
#include "DeerRender/Voxels/ChunkRender.h"
#include "DeerRender/Voxels/Voxel.h"
#include "DeerRender/Render/Camera.h"
#include "DeerRender/Scene/SceneCamera.h"
#endif

#include "glm/glm.hpp"
#include <vector>
#include <queue>

namespace Deer {
	class Chunk;
	class Layer;

	class VoxelWorld {
	public:
		VoxelWorld(const VoxelWorldProps& props);
		~VoxelWorld();

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
		Chunk* m_chunks;
		Layer* m_layers;

		LayerVoxel& modLayerVoxel(int x, int z);
#ifdef DEER_RENDER
	public:
		void render(SceneCamera camera);

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
		// Chunk render data
		ChunkRender* m_chunksRender;
		ChunkUpdateQueue m_chunkQueue;

		// Voxel creation data
		std::vector<uint32_t> m_indices;
		std::vector<SolidVoxelVertexData> m_vertexData;
		std::vector<VoxelCordinates> tmp_voxelLightSource;
		std::queue<VoxelCordinates> m_ambientLightPropagation;
		std::queue<VoxelCordinates> m_voxelLightPropagation;

		// Chunk vertex creation
		void genSolidVoxel(ChunkID chunkID, ChunkVoxelID chunkVoxelID);

		//Light propagation
		void resolveNextAmbientLightPropagation();
		void resolveNextVoxelLightPropagation();
#endif
	};
}

