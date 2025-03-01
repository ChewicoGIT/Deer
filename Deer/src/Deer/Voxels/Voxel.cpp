#include "Voxel.h"

namespace Deer {
	// This means the voxel is null
	Voxel nullVoxel(65535);
	Voxel emptyVoxel;

	LayerVoxel nullLayerVoxel(65535);

	int normalDirs[3 * 6] = {
		-1, 0, 0,
		 1, 0, 0,
		0, -1, 0,
		0,  1, 0,
		0, 0, -1,
		0, 0,  1
	};

}
