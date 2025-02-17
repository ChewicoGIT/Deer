#pragma once
#include "Deer/Voxels/Voxel.h"

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
	private:
		void loadData();

		LayerVoxel* m_layerInfo = nullptr;
	};
}