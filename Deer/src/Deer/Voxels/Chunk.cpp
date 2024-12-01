#include "Chunk.h"

namespace Deer {
	Chunk::Chunk() : m_voxels(nullptr) { }
	Chunk::~Chunk() {
		if (m_voxels)
			delete[] m_voxels;
	}

	void Chunk::loadVoxels() {
		m_voxels = new Voxel[CHUNK_VOXELS];
	}
}
