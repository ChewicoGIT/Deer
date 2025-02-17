#include "Voxel.h"


#include <iostream>
#include <string>

namespace Deer {
	VoxelLight lightVoxel(255);

	int lightPropagationComplexDir[12 * 2] = {
		NORMAL_LEFT,	NORMAL_DOWN,
		NORMAL_RIGHT,	NORMAL_DOWN,
		NORMAL_LEFT,	NORMAL_UP,
		NORMAL_RIGHT,	NORMAL_UP,
		NORMAL_LEFT,	NORMAL_BACK,
		NORMAL_RIGHT,	NORMAL_BACK,
		NORMAL_LEFT,	NORMAL_FRONT,
		NORMAL_RIGHT,	NORMAL_FRONT,
		NORMAL_DOWN,	NORMAL_BACK,
		NORMAL_UP,		NORMAL_BACK,
		NORMAL_DOWN,	NORMAL_FRONT,
		NORMAL_UP,		NORMAL_FRONT
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
		1, 0,
		0, 1,
		1, 1
	};

	int ambientOcclusionVertex[6 * 2 * 4 * 3]{
		0, -1, 0,       0, 0, 1,
		0, -1, 0,       0, 0, -1,
		0, 1, 0,        0, 0, 1,
		0, 1, 0,        0, 0, -1,
		0, -1, 0,       0, 0, -1,
		0, -1, 0,       0, 0, 1,
		0, 1, 0,        0, 0, -1,
		0, 1, 0,        0, 0, 1,
		-1, 0, 0,       0, 0, 1,
		1, 0, 0,        0, 0, 1,
		-1, 0, 0,       0, 0, -1,
		1, 0, 0,        0, 0, -1,
		-1, 0, 0,       0, 0, -1,
		1, 0, 0,        0, 0, -1,
		-1, 0, 0,       0, 0, 1,
		1, 0, 0,        0, 0, 1,
		-1, 0, 0,       0, -1, 0,
		1, 0, 0,        0, -1, 0,
		-1, 0, 0,       0, 1, 0,
		1, 0, 0,        0, 1, 0,
		1, 0, 0,        0, -1, 0,
		-1, 0, 0,       0, -1, 0,
		1, 0, 0,        0, 1, 0,
		-1, 0, 0,       0, 1, 0
	};

	void calcFaces() {
		std::string out;

		for (int f = 0; f < 6; f++) {
			int normalDirX = NORMAL_DIR(X_AXIS, f);
			int normalDirY = NORMAL_DIR(Y_AXIS, f);
			int normalDirZ = NORMAL_DIR(Z_AXIS, f);

			for (int vertex = 0; vertex < 4; vertex++) {
				int xPos = NORMAL_VERTEX_POS(X_AXIS, vertex, f);
				int yPos = NORMAL_VERTEX_POS(Y_AXIS, vertex, f);
				int zPos = NORMAL_VERTEX_POS(Z_AXIS, vertex, f);

				int sideX = (xPos == 1) ? 1 : -1;
				int sideY = (yPos == 1) ? 1 : -1;
				int sideZ = (zPos == 1) ? 1 : -1;

				if (normalDirX == 1 || normalDirX == -1) {
					out += std::to_string(0) + ", ";
					out += std::to_string(sideY) + ", ";
					out += std::to_string(0) + ",\t";

					out += std::to_string(0) + ", ";
					out += std::to_string(0) + ", ";
					out += std::to_string(sideZ) + ",\n";
				}

				if (normalDirY == 1 || normalDirY == -1) {
					out += std::to_string(sideX) + ", ";
					out += std::to_string(0) + ", ";
					out += std::to_string(0) + ",\t";

					out += std::to_string(0) + ", ";
					out += std::to_string(0) + ", ";
					out += std::to_string(sideZ) + ",\n";

				}

				if (normalDirZ == 1 || normalDirZ == -1) {
					out += std::to_string(sideX) + ", ";
					out += std::to_string(0) + ", ";
					out += std::to_string(0) + ",\t";

					out += std::to_string(0) + ", ";
					out += std::to_string(sideY) + ", ";
					out += std::to_string(0) + ",\n";

				}
			}
		}

		std::cout << out;
	}/**/
}