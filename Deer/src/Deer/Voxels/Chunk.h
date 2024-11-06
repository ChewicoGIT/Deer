#pragma once
#include "Voxel.h"

namespace Deer {
	class Chunk {
	public:
		void initializeEmpty();

		inline Voxel& getVoxel(int x, int y, int z) { return voxelData[VOXEL_POSITION(x, y, z)]; }
		bool isDataLoaded() { return voxelData != nullptr; }
	private:
		Voxel* voxelData = nullptr;

		void generateData();
	};
}

