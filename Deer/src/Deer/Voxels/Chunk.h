#pragma once
#include "Voxel.h"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/Voxel.h"
#include "DeerRender/Voxels/VoxelAspect.h"
#include "Deer/Voxels/VoxelData.h"
#include <vector>
#endif

#include <array>

namespace Deer {
	class Chunk {
	public:
		Chunk() = default;
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

		inline void fillVoxels(ChunkVoxelID min, ChunkVoxelID max, Voxel info) {
			if (!m_voxels)
				loadVoxels();

			ChunkVoxelID voxelID;
			for (voxelID.x = min.x; voxelID.x <= max.x; voxelID.x++) {
				for (voxelID.y = min.y; voxelID.y <= max.y; voxelID.y++) {
					for (voxelID.z = min.z; voxelID.z <= max.x; voxelID.z++) {
						m_voxels[VOXEL_POSITION(voxelID)] = info;
					}
				}
			}
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
			if (m_voxels)
				return m_lightInfo[VOXEL_POSITION(id)];
			return VoxelLight();
		}

		inline VoxelLight& modLight(ChunkVoxelID id) {
			if (!m_voxels)
				loadVoxels();
			return m_lightInfo[VOXEL_POSITION(id)];
		}

		inline void clearVoxelLight(ChunkVoxelID min, ChunkVoxelID max) {
			ChunkVoxelID voxelID;
			for (voxelID.x = min.x; voxelID.x <= max.x; voxelID.x++){
				for (voxelID.y = min.y; voxelID.y <= max.y; voxelID.y++){
					for (voxelID.z = min.z; voxelID.z <= max.z; voxelID.z++){
						m_lightInfo[VOXEL_POSITION(voxelID)].b_light = 0;
						m_lightInfo[VOXEL_POSITION(voxelID)].r_light = 0;
						m_lightInfo[VOXEL_POSITION(voxelID)].g_light = 0;
					}
				}
			}
		}

		// This function is the same as clear Voxel Light but it also checks if there is a source of light
		inline void clearVoxelLightAndSaveSources(ChunkVoxelID min, ChunkVoxelID max, ChunkID chunkID, std::vector<VoxelCordinates>& sources) {
			if (!m_voxels)
				return;
			
			ChunkVoxelID voxelID;
			for (voxelID.x = min.x; voxelID.x <= max.x; voxelID.x++){
				for (voxelID.y = min.y; voxelID.y <= max.y; voxelID.y++){
					for (voxelID.z = min.z; voxelID.z <= max.z; voxelID.z++){
						Voxel voxel = m_voxels[VOXEL_POSITION(voxelID)];
						VoxelAspect& voxelAspect = VoxelData::voxelsAspect[voxel.id];
						if (voxelAspect.isLightSource()) {
							sources.push_back(VoxelCordinates(
								voxelID.x + chunkID.x * CHUNK_SIZE_X,
								voxelID.y + chunkID.y * CHUNK_SIZE_Y,
								voxelID.z + chunkID.z * CHUNK_SIZE_Z
							));
						}

						m_lightInfo[VOXEL_POSITION(voxelID)].b_light = 0;
						m_lightInfo[VOXEL_POSITION(voxelID)].r_light = 0;
						m_lightInfo[VOXEL_POSITION(voxelID)].g_light = 0;
					}
				}
			}
		}
	private:
		VoxelLight* m_lightInfo = nullptr;
#endif
	};
}

