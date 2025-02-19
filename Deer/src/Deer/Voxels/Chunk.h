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

		inline uint8_t getLayerVoxelHeight(LayerVoxelID layerVoxelID) {
			if (!m_voxels)
				return 0;

			ChunkVoxelID voxelID(layerVoxelID.x, CHUNK_SIZE_Y - 1, layerVoxelID.z);
			for (int y = CHUNK_SIZE_Y - 1; y >= 0; y--) {
				voxelID.y = y;

				if (m_voxels[VOXEL_POSITION(voxelID)].id != 0)
					return voxelID.y + 1;
			}
			return 0;
		}
	private:
		Voxel* m_voxels = nullptr;

		void loadVoxels();
#ifdef DEER_RENDER
	public:
		inline VoxelLight readLight(ChunkVoxelID id) {
			return m_lightInfo[VOXEL_POSITION(id)];
		}

		inline VoxelLight& modLight(ChunkVoxelID id) {
			return m_lightInfo[VOXEL_POSITION(id)];
		}
	private:
		VoxelLight* m_lightInfo = nullptr;
#endif
	};
}

