#pragma once
#include "Deer/Core/Core.h"
#include "Voxel.h"
#include <array>

namespace Deer {
	class Chunk {
	public:
		Chunk();
		~Chunk();

		inline Voxel readVoxel(ChunkVoxelID id) {
			if (m_voxels)
				return m_voxels[VOXEL_POSITION(id)];
			return emptyVoxel;
		}

		inline Voxel& modVoxel(ChunkVoxelID id) {
			if (!m_voxels)
				loadVoxels();
			return m_voxels[VOXEL_POSITION(id)];
		}
	private:
		Voxel* m_voxels;

		void loadVoxels();
	};
}

