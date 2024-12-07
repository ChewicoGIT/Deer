#include "Voxel.h"

namespace Deer {
	// This means the voxel is null
	Voxel nullVoxel = Voxel(65535);
	Voxel emptyVoxel = Voxel();

	int normalDirs[3 * 6] = {
		-1, 0, 0,
		 1, 0, 0,
		0, -1, 0,
		0,  1, 0,
		0, 0, -1,
		0, 0,  1
	};

	int normalFacePositions[3 * 4 * 6] = {
		// Left
		0, 0, 1,
		0, 0, 0,
		0, 1, 1,
		0, 1, 0,
		// Right
		1, 0, 0,
		1, 0, 1,
		1, 1, 0,
		1, 1, 1,
		// Down
		0, 0, 1,
		1, 0, 1,
		0, 0, 0,
		1, 0, 0,
		// Up
		0, 1, 0,
		1, 1, 0,
		0, 1, 1,
		1, 1, 1,
		// Back
		0, 0, 0,
		1, 0, 0,
		0, 1, 0,
		1, 1, 0,
		// Front
		1, 0, 1,
		0, 0, 1,
		1, 1, 1,
		0, 1, 1
	};
	int uvFace[2 * 4] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};
}
