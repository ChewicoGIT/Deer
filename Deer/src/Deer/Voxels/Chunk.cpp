#include "Chunk.h"

namespace Deer {
	Chunk::Chunk() {
		m_voxelData = new Voxel[CHUNK_VOXELS];
	}
	Chunk::~Chunk() {
		delete[] m_voxelData;
	}

}
