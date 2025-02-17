#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Voxels/Voxel.h"

#define LIGHT_PROPAGATION_COMPLEX_DIRS 18
#define LIGHT_PROPAGATION_COMPLEX_DIR(id, dir) lightPropagationComplexDir[id + dir * 2]
#define LIGHT_PROPAGATION_SIMPLE_FALL 16
#define LIGHT_PROPAGATION_COMPLEX_FALL 23

#define NORMAL_VERTEX_POS(axis, id, normal) normalFacePositions[axis + id * 3 + normal * 3 * 4]
#define VERTEX_UV(axis, id) uvFace[axis + id * 2]
#define AMBIENT_OCCLUSION_VERTEX(axis, id, vertex, normal) ambientOcclusionVertex[axis + id * 3 + vertex * 3 * 2 + normal * 3 * 2 * 4]

namespace Deer {
	struct VoxelLight;
	extern VoxelLight lightVoxel;

	extern int lightPropagationComplexDir[12 * 2];
	extern int normalFacePositions[3 * 4 * 6];
	extern int uvFace[2 * 4];
	// 6 Dirs * 4 vertices * 2 checks * 3 dirs
	extern int ambientOcclusionVertex[6 * 4 * 2 * 3];

	struct VoxelLight {
		uint8_t r_light;
		uint8_t g_light;
		uint8_t b_light;
		uint8_t ambient_light;

		VoxelLight(uint8_t _ambient_light = 0) : r_light(0), g_light(0), b_light(0), ambient_light(_ambient_light) { }
	};
}