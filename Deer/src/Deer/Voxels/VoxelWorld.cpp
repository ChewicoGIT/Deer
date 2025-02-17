#include "VoxelWorld.h"
#include "Deer/Core/Log.h"

#include <math.h>
#include <cmath>
#include <vector>

namespace Deer {
	VoxelWorld::VoxelWorld(const VoxelWorldProps& props) 
		: m_worldProps(props) {
		m_chunks = new Chunk[m_worldProps.getChunkCount()]();
		m_layers = new Layer[m_worldProps.getLayerCount()]();
#ifdef DEER_RENDER
		m_chunksRender = new ChunkRender[m_worldProps.getChunkCount()];
#endif
	}

	VoxelWorld::~VoxelWorld() {
		delete[] m_chunks;
		delete[] m_layers;
#ifdef DEER_RENDER
		delete[] m_chunksRender;
#endif
	}

	Voxel VoxelWorld::readVoxel(int x, int y, int z) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return emptyVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		return chunk.readVoxel(chunkVoxelID);
	}

	void VoxelWorld::setVoxel(int x, int y, int z, Voxel info) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(x, y, z, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return;

#ifdef DEER_RENDER
		m_chunkQueue.addChunk(chunkID);
#endif
		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		chunk.modVoxel(chunkVoxelID) = info;

		LayerID layerID;
		LayerVoxelID layerVoxelID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);

		Layer& layer = m_layers[m_worldProps.getWorldLayerID(layerID)];
		LayerVoxel& layerVoxel = layer.modLayerVoxel(layerVoxelID);

		if (info.id == 0)
			layerVoxel.height = getLayerVoxelHeight(x, z);
		else if (y >= layerVoxel.height)
			layerVoxel.height = y + 1;

	}

	LayerVoxel VoxelWorld::readLayerVoxel(int x, int z) {
		LayerID layerID;
		LayerVoxelID layerVoxelID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);
		if (!m_worldProps.isValid(layerID))
			return LayerVoxel();

		Layer& layer = m_layers[m_worldProps.getWorldLayerID(layerID)];
		return layer.readLayerVoxel(layerVoxelID);
	}

	LayerVoxel& VoxelWorld::modLayerVoxel(int x, int z) {
		LayerID layerID;
		LayerVoxelID layerVoxelID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);
		if (!m_worldProps.isValid(layerID))
			return nullLayerVoxel;

		Layer& layer = m_layers[m_worldProps.getWorldLayerID(layerID)];
		return layer.modLayerVoxel(layerVoxelID);
	}

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

	uint16_t VoxelWorld::getLayerVoxelHeight(int x, int z) {
		LayerVoxelID layerVoxelID;
		LayerID layerID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);
		ChunkID chunkID(layerID.x, m_worldProps.chunkSizeY - 1, layerID.z);

		for (; chunkID.y > 0; chunkID.y--) {
			Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
			uint8_t chunkVoxelHeight = chunk.getLayerVoxelHeight(layerVoxelID);

			if (chunkVoxelHeight != 0) {
				return chunkVoxelHeight + chunkID.y * CHUNK_SIZE_Y;
			}
		}
		return 0;
	}
}
