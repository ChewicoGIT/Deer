#pragma once
#include "Deer/Voxel.h"

namespace Deer {
	class Layer {
	public:
		Layer() = default;
		~Layer();

		inline LayerVoxel readLayerVoxel(LayerVoxelID id) {
			if (!m_layerInfo)
				return LayerVoxel();
			return m_layerInfo[LAYER_VOXEL_POSITION(id)];
		}

		inline LayerVoxel& modLayerVoxel(LayerVoxelID id) {
			if (!m_layerInfo)
				loadData();
			return m_layerInfo[LAYER_VOXEL_POSITION(id)];
		}

		inline void fillVoxelLayerMaxHeight(LayerVoxelID min, LayerVoxelID max, uint8_t maxHeight) {
			if (!m_layerInfo)
				loadData();

			LayerVoxelID layerVoxelID;
			for (layerVoxelID.x = min.x; layerVoxelID.x <= max.x; layerVoxelID.x++) {
				for (layerVoxelID.z = min.x; layerVoxelID.z <= max.z; layerVoxelID.z++) {
					int id = LAYER_VOXEL_POSITION(layerVoxelID);
					if (m_layerInfo[id].height <= maxHeight)
						m_layerInfo[id].height = maxHeight + 1;
				}
			}
		}
	private:
		void loadData();

		LayerVoxel* m_layerInfo = nullptr;
	};
}