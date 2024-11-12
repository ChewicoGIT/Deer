#pragma once
#include <unordered_map>
#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/Chunk.h"

#ifndef DEER_SERVICE
#include "DeerRender/Voxels/ChunkRender.h"
#include "DeerRender/Voxels/ChunkBaker.h"
#include "DeerRender/Render/Camera.h"
#include "Deer/Scene/SceneCamera.h"
#endif

namespace Deer {
	class VoxelWorld {
	public:
		Voxel& getVoxel(int x, int y, int z);
		void loadEmptyChunk(int x, int y, int z);

#ifndef DEER_SERVICE
	public:
		void render(SceneCamera camera);
		void bakeChunk(int x, int y, int z);

	private:
		std::unordered_map<ChunkID, ChunkRender, ChunkIDHash> m_chunksRender;
		ChunkBaker m_chunkBaker;

		void bakeChunk(ChunkID chunkID);
#endif
	private:
		std::unordered_map<ChunkID, Chunk, ChunkIDHash> m_chunks;

		inline void extractCordinates(int x, int y, int z, ChunkID& _chunkID, ChunkVoxelID& _chunkVoxelID) {
			uint16_t posX = x + (1 << 15);
			uint16_t posY = y + (1 << 15);
			uint16_t posZ = z + (1 << 15);

			_chunkID.x = posX >> 5;
			_chunkID.y = posY >> 5;
			_chunkID.z = posZ >> 5;

			_chunkVoxelID.x = posX & 31;
			_chunkVoxelID.y = posY & 31;
			_chunkVoxelID.z = posZ & 31;
		}
	};
}

