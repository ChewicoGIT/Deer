#include "Chunk.h"

namespace Deer {
	void Chunk::initializeEmpty() {
		generateData();

		for (int i = 0; i < CHUNK_VOXELS; i++) {
			voxelData[i] = Voxel();
		}

		// Temp
		voxelData[0].id = 1;
	}
	void Chunk::generateData() {
		voxelData = new Voxel[CHUNK_VOXELS];
	}
}
