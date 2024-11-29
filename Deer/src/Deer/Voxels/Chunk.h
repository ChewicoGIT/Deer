#pragma once
#include "Voxel.h"
#include <array>

namespace Deer {
	class Chunk {
	public:
		Chunk();
		~Chunk();

		inline Voxel& getVoxel(ChunkVoxelID id) { return m_voxels[VOXEL_POSITION(id)]; }
	private:
		std::array<Voxel, CHUNK_VOXELS> m_voxels;
	};
}

