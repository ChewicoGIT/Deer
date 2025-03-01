#include "VoxelWorld.h"
#include "Deer/Core/Log.h"

#include <math.h>
#include <cmath>
#include <vector>

namespace Deer {
	VoxelRayResult VoxelWorld::rayCast(glm::vec3 position, glm::vec3 dir, float maxDistance) {
		VoxelRayResult result;

		result.xPos = (int32_t)std::floor(position.x);
		result.yPos = (int32_t)std::floor(position.y);
		result.zPos = (int32_t)std::floor(position.z);

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
				distanceAxis[i] = stepAxis[i] * ((float)position[i] - (float)(&result.xPos)[i]);
			}
			else if (dir[i] > 0) {
				stepAxis[i] = 1.0f / dir[i];
				distanceAxis[i] = stepAxis[i] * (1 - (float)position[i] + (float)(&result.xPos)[i]);
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
					(&result.xPos)[i] += directionAxis[i];
					distanceAxis[i] = minDistance + stepAxis[i];

					Voxel hitVoxel = readVoxel(result.xPos, result.yPos, result.zPos);

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

		result.xPos = (int32_t)std::floor(position.x);
		result.yPos = (int32_t)std::floor(position.y);
		result.zPos = (int32_t)std::floor(position.z);

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
				distanceAxis[i] = stepAxis[i] * ((float)position[i] - (float)(&result.xPos)[i]);
			}
			else if (dir[i] > 0) {
				stepAxis[i] = 1.0f / dir[i];
				distanceAxis[i] = stepAxis[i] * (1 - (float)position[i] + (float)(&result.xPos)[i]);
			}
		}

		Voxel hitVoxel = readVoxel(result.xPos, result.yPos, result.zPos);
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
					(&result.xPos)[i] += directionAxis[i];
					distanceAxis[i] = minDistance + stepAxis[i];

					Voxel hitVoxel = readVoxel(result.xPos, result.yPos, result.zPos);

					if (hitVoxel.id == 0) {
						if (has_exit_inner_walls && result.yPos < 0) {
							result.face = i * 2;

							if (directionAxis[i] == -1)
								result.face++;
	
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
