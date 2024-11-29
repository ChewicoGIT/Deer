#pragma once
#include <unordered_map>
#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/Chunk.h"
#include "VoxelWorldProps.h"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/ChunkRender.h"
#include "DeerRender/Voxels/ChunkBaker.h"
#include "DeerRender/Render/Camera.h"
#include "DeerRender/Scene/SceneCamera.h"
#endif

namespace Deer {
	class VoxelWorld {
	public:
		void initVoxelWorld(const VoxelWorldProps& props);

		Voxel& getVoxel(int x, int y, int z);

		inline const VoxelWorldProps& getVoxelWorldProps() { return m_worldProps };
	private:
		VoxelWorldProps m_worldProps;
		Chunk* m_chunks;

#ifdef DEER_RENDER
	public:
		void render(SceneCamera camera);
		void bakeChunk(int x, int y, int z);

	private:
		std::unordered_map<ChunkID, ChunkRender, ChunkIDHash> m_chunksRender;
		ChunkBaker m_chunkBaker;

		void bakeChunk(ChunkID chunkID);
#endif
	};
}

