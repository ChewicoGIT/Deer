#pragma once
#include "Deer/Core/Core.h"
#include "Voxel.h"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/Voxel.h"
#endif

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
		Voxel* m_voxels = nullptr;

		void loadVoxels();
#ifdef DEER_RENDER
	public:
		inline VoxelLight readLight(ChunkVoxelID id) {
			if (m_lightInfo)
				return m_lightInfo[VOXEL_POSITION(id)];
			return lightVoxel;
		}

		inline VoxelLight& modLight(ChunkVoxelID id) {
			if (!m_voxels)
				loadVoxels();
			return m_lightInfo[VOXEL_POSITION(id)];
		}
	private:
		VoxelLight* m_lightInfo = nullptr;
#endif
	};
}

