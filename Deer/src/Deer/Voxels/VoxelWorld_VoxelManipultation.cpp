#include "Deer/VoxelWorld.h"
#include "Deer/Log.h"
#include "Deer/Voxels/Chunk.h"
#include "Deer/Voxels/Layer.h"

#ifdef DEER_RENDER
#include "DeerRender/Voxels/VoxelWorldRenderData.h"
#endif

#include <math.h>
#include <cmath>
#include <vector>

namespace Deer {
	Voxel VoxelWorld::readVoxel(VoxelCordinates coords) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(coords, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return emptyVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		return chunk.readVoxel(chunkVoxelID);
	}

	void VoxelWorld::setVoxel(VoxelCordinates coords, Voxel info) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(coords, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID))
			return;

#ifdef DEER_RENDER
		m_renderData->chunkQueue.addChunk(chunkID);
		if (chunkVoxelID.x == 0 && chunkID.x != 0)
			m_renderData->chunkQueue.addChunk(ChunkID(chunkID.x - 1, chunkID.y, chunkID.z));
		if (chunkVoxelID.x == CHUNK_SIZE_X - 1 && chunkID.x != m_worldProps.chunkSizeX - 1)
			m_renderData->chunkQueue.addChunk(ChunkID(chunkID.x + 1, chunkID.y, chunkID.z));

		if (chunkVoxelID.y == 0 && chunkID.y != 0)
			m_renderData->chunkQueue.addChunk(ChunkID(chunkID.x, chunkID.y - 1, chunkID.z));
		if (chunkVoxelID.y == CHUNK_SIZE_Y - 1 && chunkID.y != m_worldProps.chunkSizeY - 1)
			m_renderData->chunkQueue.addChunk(ChunkID(chunkID.x, chunkID.y + 1, chunkID.z));

		if (chunkVoxelID.z == 0 && chunkID.z != 0)
			m_renderData->chunkQueue.addChunk(ChunkID(chunkID.x, chunkID.y, chunkID.z - 1));
		if (chunkVoxelID.z == CHUNK_SIZE_Z - 1 && chunkID.z != m_worldProps.chunkSizeZ - 1)
			m_renderData->chunkQueue.addChunk(ChunkID(chunkID.x, chunkID.y, chunkID.z + 1));
#endif
		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		chunk.modVoxel(chunkVoxelID) = info;

		LayerID layerID;
		LayerVoxelID layerVoxelID;

		extractLayerCordinates(coords.x, coords.z, layerID, layerVoxelID);

		Layer& layer = m_layers[m_worldProps.getWorldLayerID(layerID)];
		LayerVoxel& layerVoxel = layer.modLayerVoxel(layerVoxelID);

		if (!info.isVoxelType())
			layerVoxel.height = calculateLayerVoxelHeight(coords.x, coords.z);
		else if (coords.y >= layerVoxel.height)
			layerVoxel.height = coords.y + 1;
	}

	void VoxelWorld::fillVoxels(VoxelCordinates min, VoxelCordinates max, Voxel info) {
		ChunkID minChunkID;
		ChunkID maxChunkID;
		ChunkVoxelID minChunkVoxelID;
		ChunkVoxelID maxChunkVoxelID;

		extractChunkCordinates(min, minChunkID, minChunkVoxelID);
		extractChunkCordinates(max, maxChunkID, maxChunkVoxelID);
		for (int chunkX = minChunkID.x; chunkX <= maxChunkID.x; chunkX++) {
			for (int chunkY = minChunkID.y; chunkY <= maxChunkID.y; chunkY++) {
				for (int chunkZ = minChunkID.z; chunkZ <= maxChunkID.z; chunkZ++) {
					ChunkID workingChunkID(chunkX, chunkY, chunkZ);
					LayerID workingLayerID(chunkX, chunkZ);
					Chunk& workingChunk = m_chunks[m_worldProps.getWorldChunkID(workingChunkID)];
					Layer& workingLayer = m_layers[m_worldProps.getWorldLayerID(workingLayerID)];

					ChunkVoxelID workingMin(0, 0, 0);
					ChunkVoxelID workingMax(CHUNK_SIZE_X - 1, CHUNK_SIZE_Y - 1, CHUNK_SIZE_Z - 1);

					if (chunkX == minChunkID.x)
						workingMin.x = minChunkVoxelID.x;
					if (chunkY == minChunkID.y)
						workingMin.y = minChunkVoxelID.y;
					if (chunkZ == minChunkID.z)
						workingMin.z = minChunkVoxelID.z;

					if (chunkX == maxChunkID.x)
						workingMax.x = maxChunkVoxelID.x;
					if (chunkY == maxChunkID.y)
						workingMax.y = maxChunkVoxelID.y;
					if (chunkZ == maxChunkID.z)
						workingMax.z = maxChunkVoxelID.z;

					LayerVoxelID workingMinLayer(workingMin.x, workingMin.z);
					LayerVoxelID workingMaxLayer(workingMax.x, workingMax.z);

					workingChunk.fillVoxels(workingMin, workingMax, info);
					workingLayer.fillVoxelLayerMaxHeight(workingMinLayer, workingMaxLayer, max.y);

					#ifdef DEER_RENDER
					m_renderData->chunkQueue.addChunk(workingChunkID);
					#endif
				}
			}
		}

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

}
