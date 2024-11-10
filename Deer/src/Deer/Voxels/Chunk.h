#pragma once
#include "Voxel.h"

namespace Deer {
	class Chunk {
	public:
		~Chunk();
		void initializeEmpty();

		inline Voxel& getVoxel(ChunkVoxelID id) { return m_voxelData[VOXEL_POSITION(id)]; }
		bool isDataLoaded() { return m_voxelData != nullptr; }
	private:
		Voxel* m_voxelData = nullptr;

		void generateData();
	};
}

