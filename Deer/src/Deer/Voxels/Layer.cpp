#include "Layer.h"

namespace Deer {
	Layer::~Layer() {
		if (m_layerInfo)
			delete[] m_layerInfo;
	}

	void Layer::loadData() {
		m_layerInfo = new LayerVoxel[LAYER_VOXELS]();
	}
}