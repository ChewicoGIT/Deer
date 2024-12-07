#include "VoxelWorld.h"

namespace Deer {
	VoxelWorld::VoxelWorld(const VoxelWorldProps& props) 
		: m_worldProps(props) {
		m_chunks = new Chunk[m_worldProps.getChunkCount()];
#ifdef DEER_RENDER
		m_chunksRender = new ChunkRender[m_worldProps.getChunkCount()];
#endif
	}

	VoxelWorld::~VoxelWorld() {
		delete[] m_chunks;
#ifdef DEER_RENDER
		delete[] m_chunksRender;
#endif
	}

	Voxel VoxelWorld::readVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return nullVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getInternalID(chunkID)];
		return chunk.readVoxel(chunkVoxelID);
	}

	Voxel& VoxelWorld::modVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return nullVoxel;

#ifdef DEER_RENDER
		m_chunkQueue.addChunk(chunkID);
#endif
		Chunk& chunk = m_chunks[m_worldProps.getInternalID(chunkID)];
		return chunk.modVoxel(chunkVoxelID);
	}

	VoxelRayResult VoxelWorld::rayCast(glm::vec3 position, glm::vec3 dir, float maxDistance) {
		VoxelRayResult result;

		result.xPos = (uint32_t)position.x;
		result.yPos = (uint32_t)position.y;
		result.zPos = (uint32_t)position.z;

		if (dir.x == 0 && dir.y == 0 && dir.z == 0) {
			result.distance = 0;
			return result;
		}

		dir = glm::normalize(dir);
		glm::vec3 stepAxis = glm::vec3(maxDistance, maxDistance, maxDistance);
		int8_t directionAxis[3] = { 1, 1, 1 };
		for (int i = 0; i < 3; i++) {
			if (dir[i] != 0) {
				if (dir[i] < 0) {
					stepAxis[i] = -1 / dir[i];
					directionAxis[i] = -1;
				} else
					stepAxis[i] = 1 / dir[i];
			}
		}
		glm::vec3 distanceAxis = stepAxis;

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
						break;

					if (hitVoxel != 0) {
						result.face = i;
						return result;
					}
				}
			}
		}

		return result;
	}
}
