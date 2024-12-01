#include "VoxelWorld.h"

namespace Deer {
	VoxelWorld::VoxelWorld(const VoxelWorldProps& props) 
		: m_worldProps(props) {
		m_chunks = new Chunk[m_worldProps.getChunkCount()];
#ifdef DEER_RENDER
		m_chunksRender = new ChunkRender[m_worldProps.getChunkCount()];
#endif
	}

	VoxelWorld::~VoxelWorld() {
		delete[] m_chunks;
#ifdef DEER_RENDER
		delete[] m_chunksRender;
#endif
	}

	Voxel VoxelWorld::readVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return nullVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getInternalID(chunkID)];
		return chunk.readVoxel(chunkVoxelID);
	}

	Voxel& VoxelWorld::modVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return nullVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getInternalID(chunkID)];
		return chunk.modVoxel(chunkVoxelID);
	}
}
