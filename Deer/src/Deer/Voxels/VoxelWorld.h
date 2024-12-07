#pragma once
#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/Chunk.h"
#include "VoxelWorldProps.h"
#include "glm/glm.hpp"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/ChunkRender.h"
#include "DeerRender/Voxels/ChunkBaker.h"
#include "DeerRender/Voxels/ChunkUpdateQueue.h"
#include "DeerRender/Render/Camera.h"
#include "DeerRender/Scene/SceneCamera.h"
#endif

namespace Deer {
	class VoxelWorld {
	public:
		VoxelWorld(const VoxelWorldProps& props);
		~VoxelWorld();

		Voxel readVoxel(int x, int y, int z);
		Voxel& modVoxel(int x, int y, int z);
		VoxelRayResult rayCast(glm::vec3 position, glm::vec3 dir, float maxDistance = 1000.0f);

		inline const VoxelWorldProps& getVoxelWorldProps() { return m_worldProps; }
	private:
		VoxelWorldProps m_worldProps;
		Chunk* m_chunks;

#ifdef DEER_RENDER
	public:
		void render(SceneCamera camera);
		void bakeChunk(int x, int y, int z);
		void bakeNextChunk();

	private:
		ChunkRender* m_chunksRender;
		ChunkBaker m_chunkBaker;
		ChunkUpdateQueue m_chunkQueue;

		void bakeChunk(ChunkID chunkID);
#endif
	};
}

