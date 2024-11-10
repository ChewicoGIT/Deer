#include "Chunk.h"

namespace Deer {
	Chunk::~Chunk() {
		if (isDataLoaded())
			delete[] m_voxelData;
	}

	void Chunk::initializeEmpty() {
		generateData();

		for (int i = 0; i < CHUNK_VOXELS; i++) {
			m_voxelData[i] = Voxel();
		}
	}

	void Chunk::generateData() {
		m_voxelData = new Voxel[CHUNK_VOXELS];
	}
}
