#include "Deer/VoxelWorld.h"
#include "Deer/Log.h"
#include "Deer/Voxels/Chunk.h"
#include "Deer/Voxels/Layer.h"

#include <math.h>
#include <cmath>
#include <vector>

namespace Deer {
	VoxelRayResult VoxelWorld::rayCast(glm::vec3 position, glm::vec3 dir, float maxDistance) {
		VoxelRayResult result;

		result.hitPos.x = (int32_t)std::floor(position.x);
		result.hitPos.y = (int32_t)std::floor(position.y);
		result.hitPos.z = (int32_t)std::floor(position.z);

		result.distance = 0;

		if (dir.x == 0 && dir.y == 0 && dir.z == 0) {
			return result;
		}

		dir = glm::normalize(dir);

		glm::vec3 stepAxis = glm::vec3(maxDistance, maxDistance, maxDistance);
		glm::vec3 distanceAxis = glm::vec3(maxDistance, maxDistance, maxDistance);

		int8_t directionAxis[3] = { 1, 1, 1 };

		for (int i = 0; i < 3; i++) {
			if (dir[i] < 0) {
				stepAxis[i] = -1.0f / dir[i];
				directionAxis[i] = -1;
				distanceAxis[i] = stepAxis[i] * ((float)position[i] - (float)(&result.hitPos.x)[i]);
			}
			else if (dir[i] > 0) {
				stepAxis[i] = 1.0f / dir[i];
				distanceAxis[i] = stepAxis[i] * (1 - (float)position[i] + (float)(&result.hitPos.x)[i]);
			}
		}

		while (result.distance < maxDistance) {
			float minDistance = distanceAxis[0];
			for (int i = 1; i < 3; i++) {
				if (distanceAxis[i] < minDistance)
					minDistance = distanceAxis[i];
			}

			result.distance = minDistance;
			if (result.distance > maxDistance)
				break;

			for (int i = 0; i < 3; i++) {
				if (minDistance == distanceAxis[i]) {
					result.hitPos[i] += directionAxis[i];
					distanceAxis[i] = minDistance + stepAxis[i];

					Voxel hitVoxel = readVoxel(result.hitPos);

					if (hitVoxel == nullVoxel)
						continue;

					if (hitVoxel != 0) {
						result.face = i * 2;

						if (directionAxis[i] == -1)
							result.face++;

						return result;
					}
				}
			}
		}

		result.distance = maxDistance;
		return result;
	}

	VoxelRayResult VoxelWorld::rayCast_editor(glm::vec3 position, glm::vec3 dir, float maxDistance) {
		VoxelRayResult result;

		result.hitPos.x = (int32_t)std::floor(position.x);
		result.hitPos.y = (int32_t)std::floor(position.y);
		result.hitPos.z = (int32_t)std::floor(position.z);

		result.distance = 0;

		if (dir.x == 0 && dir.y == 0 && dir.z == 0) {
			return result;
		}

		dir = glm::normalize(dir);

		glm::vec3 stepAxis = glm::vec3(maxDistance, maxDistance, maxDistance);
		glm::vec3 distanceAxis = glm::vec3(maxDistance, maxDistance, maxDistance);

		int8_t directionAxis[3] = { 1, 1, 1 };

		for (int i = 0; i < 3; i++) {
			if (dir[i] < 0) {
				stepAxis[i] = -1.0f / dir[i];
				directionAxis[i] = -1;
				distanceAxis[i] = stepAxis[i] * ((float)position[i] - (float)result.hitPos[i]);
			}
			else if (dir[i] > 0) {
				stepAxis[i] = 1.0f / dir[i];
				distanceAxis[i] = stepAxis[i] * (1 - (float)position[i] + (float)result.hitPos[i]);
			}
		}

		Voxel hitVoxel = readVoxel(result.hitPos);
		bool has_exit_inner_walls = hitVoxel.id == 0;
		while (result.distance < maxDistance) {
			float minDistance = distanceAxis[0];
			for (int i = 1; i < 3; i++) {
				if (distanceAxis[i] < minDistance)
					minDistance = distanceAxis[i];
			}

			result.distance = minDistance;
			if (result.distance > maxDistance)
				break;

			for (int i = 0; i < 3; i++) {
				if (minDistance == distanceAxis[i]) {
					result.hitPos[i] += directionAxis[i];
					distanceAxis[i] = minDistance + stepAxis[i];

					Voxel hitVoxel = readVoxel(result.hitPos);

					if (hitVoxel.id == 0) {
						if (has_exit_inner_walls && result.hitPos.y == -1 && directionAxis[1] == -1 && i == 1) {
							result.face = NORMAL_UP;
							return result;
						}

						has_exit_inner_walls = true;
					} else if (hitVoxel.id != 0 && has_exit_inner_walls) {
						result.face = i * 2;

						if (directionAxis[i] == -1)
							result.face++;

						return result;
					}
				}
			}
		}

		result.distance = maxDistance;
		return result;
	}
}
