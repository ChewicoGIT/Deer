#include "Chunk.h"

namespace Deer {
	Chunk::Chunk() {
#ifdef DEER_RENDER
		m_lightInfo = new VoxelLight[CHUNK_VOXELS];
#endif
	}
	Chunk::~Chunk() {
		if (m_voxels)
			delete[] m_voxels;
#ifdef DEER_RENDER
		delete[] m_lightInfo;
#endif
	}

	void Chunk::loadVoxels() {
		m_voxels = new Voxel[CHUNK_VOXELS]();
	}
}
