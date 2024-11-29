#include "VoxelWorld.h"

namespace Deer {
	void VoxelWorld::initVoxelWorld(const VoxelWorldProps& props) {
		m_worldProps = props;
		m_chunks = new Chunk[m_worldProps.getChunkCount()];
	}

	Voxel& VoxelWorld::getVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return nullVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getInternalID(chunkID)];
		return chunk.getVoxel(chunkVoxelID);
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
