#include "VoxelWorld.h"

namespace Deer {
	Voxel& VoxelWorld::getVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractCordinates(x, y, z, chunkID, chunkVoxelID);

		// If chunk exists
		if (m_chunks.contains(chunkID)) {
			Chunk& chunk = m_chunks[chunkID];
			
			// If chunk has data return the data
			if (chunk.isDataLoaded())
				return chunk.getVoxel(chunkVoxelID);

			// If it does not have the data return null and ask for data loaded
			return nullVoxel;
		}

		return nullVoxel;
	}

	void VoxelWorld::loadEmptyChunk(int x, int y, int z) {
		ChunkID chunkID;
		chunkID.x = x + (1 << 10);
		chunkID.y = y + (1 << 10);
		chunkID.z = z + (1 << 10);

		if (m_chunks.contains(chunkID))
			return;

		m_chunks.insert({ chunkID, {} });
		Chunk& chunk = m_chunks[chunkID];
		chunk.initializeEmpty();
	}
}
