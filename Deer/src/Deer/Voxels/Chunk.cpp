#include "Chunk.h"

namespace Deer {
	Chunk::Chunk() : m_voxels() { }
	Chunk::~Chunk() {
		if (m_voxels)
			delete[] m_voxels;
#ifdef DEER_RENDER
		if (m_lightInfo)
			delete[] m_lightInfo;
#endif
	}

	void Chunk::loadVoxels() {
		m_voxels = new Voxel[CHUNK_VOXELS];
#ifdef DEER_RENDER
		m_lightInfo = new VoxelLight[CHUNK_VOXELS];
#endif
	}
}
