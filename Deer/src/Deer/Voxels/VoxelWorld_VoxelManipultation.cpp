#include "Deer/Log.h"
#include "Deer/VoxelWorld.h"
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
		if (!m_worldProps.isValid(chunkID)) return emptyVoxel;

		Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
		return chunk.readVoxel(chunkVoxelID);
	}

	void VoxelWorld::setVoxel(VoxelCordinates coords, Voxel info) {
		ChunkID chunkID;
		ChunkVoxelID chunkVoxelID;

		extractChunkCordinates(coords, chunkID, chunkVoxelID);
		if (!m_worldProps.isValid(chunkID)) return;

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

#ifdef DEER_RENDER
		m_renderData->chunkQueue.addChunk(chunkID);
		// For every axis, X & Y & Z
		for (int i = 0; i < 3; i++) {
			if (chunkVoxelID[i] == 0 && chunkID[i] != 0) {
				ChunkID nextChunk = chunkID;
				nextChunk[i]--;
				m_renderData->chunkQueue.addChunk(nextChunk);
			}

			if (chunkVoxelID[i] == CHUNK_SIZE(i) &&
			    chunkID[i] != m_worldProps[i] - 1) {
				ChunkID nextChunk = chunkID;
				nextChunk[i]++;
				m_renderData->chunkQueue.addChunk(nextChunk);
			}
		}

		// Check if we should update the lighting
		bakeAmbientLightFromPoint(coords.x, coords.z);
		bakeVoxelLightFromPoint(coords);
#endif
	}

	void VoxelWorld::fillVoxels(VoxelCordinates min, VoxelCordinates max,
	                            Voxel info) {
		ChunkID minChunkID;
		ChunkID maxChunkID;
		ChunkVoxelID minChunkVoxelID;
		ChunkVoxelID maxChunkVoxelID;

		m_worldProps.clampAndSetMinMax(min, max);

		extractChunkCordinates(min, minChunkID, minChunkVoxelID);
		extractChunkCordinates(max, maxChunkID, maxChunkVoxelID);
		for (int chunkX = minChunkID.x; chunkX <= maxChunkID.x; chunkX++) {
			for (int chunkY = minChunkID.y; chunkY <= maxChunkID.y; chunkY++) {
				for (int chunkZ = minChunkID.z; chunkZ <= maxChunkID.z;
				     chunkZ++) {
					ChunkID workingChunkID(chunkX, chunkY, chunkZ);
					LayerID workingLayerID(chunkX, chunkZ);
					Chunk& workingChunk =
					    m_chunks[m_worldProps.getWorldChunkID(workingChunkID)];
					Layer& workingLayer =
					    m_layers[m_worldProps.getWorldLayerID(workingLayerID)];

					ChunkVoxelID workingMin(0, 0, 0);
					ChunkVoxelID workingMax(CHUNK_SIZE_X - 1, CHUNK_SIZE_Y - 1,
					                        CHUNK_SIZE_Z - 1);

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
					workingLayer.fillVoxelLayerMaxHeight(
					    workingMinLayer, workingMaxLayer, max.y);

#ifdef DEER_RENDER
					m_renderData->chunkQueue.addChunk(workingChunkID);
#endif
				}
			}
		}

#ifdef DEER_RENDER
		VoxelCordinates minLightModification = min;
		VoxelCordinates maxLightModification = max;
		// We want to add a 16 layer border
		for (int i = 0; i < 3; i++) {
			minLightModification[i] -= 16;
			maxLightModification[i] += 16;
		}

		m_worldProps.clampCordinates(minLightModification);
		m_worldProps.clampCordinates(maxLightModification);

		bakeAmbientLight(minLightModification.x, maxLightModification.x,
		                 minLightModification.z, maxLightModification.z);
		bakeVoxelLight(minLightModification, maxLightModification);
#endif
	}

	void VoxelWorld::remplaceVoxels(VoxelCordinates min, VoxelCordinates max,
	                                Voxel ref, Voxel value) {
		ChunkID minChunkID;
		ChunkID maxChunkID;
		ChunkVoxelID minChunkVoxelID;
		ChunkVoxelID maxChunkVoxelID;

		m_worldProps.clampAndSetMinMax(min, max);

		extractChunkCordinates(min, minChunkID, minChunkVoxelID);
		extractChunkCordinates(max, maxChunkID, maxChunkVoxelID);
		for (int chunkX = minChunkID.x; chunkX <= maxChunkID.x; chunkX++) {
			for (int chunkY = minChunkID.y; chunkY <= maxChunkID.y; chunkY++) {
				for (int chunkZ = minChunkID.z; chunkZ <= maxChunkID.z;
				     chunkZ++) {
					ChunkID workingChunkID(chunkX, chunkY, chunkZ);
					Chunk& workingChunk =
					    m_chunks[m_worldProps.getWorldChunkID(workingChunkID)];

					ChunkVoxelID workingMin(0, 0, 0);
					ChunkVoxelID workingMax(CHUNK_SIZE_X - 1, CHUNK_SIZE_Y - 1,
					                        CHUNK_SIZE_Z - 1);

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

					workingChunk.remplaceVoxels(workingMin, workingMax, ref,
					                            value);

#ifdef DEER_RENDER
					m_renderData->chunkQueue.addChunk(workingChunkID);
#endif
				}
			}
		}

		for (int xPos = min.x; xPos <= max.x; xPos++) {
			for (int zPos = min.z; zPos <= max.z; zPos++) {
				LayerID layerID;
				LayerVoxelID layerVoxelID;

				extractLayerCordinates(xPos, zPos, layerID, layerVoxelID);
				int worldLayerID = m_worldProps.getWorldLayerID(layerID);

				m_layers[worldLayerID].modLayerVoxel(layerVoxelID).height =
				    calculateLayerVoxelHeight(xPos, zPos);
			}
		}

#ifdef DEER_RENDER
		VoxelCordinates minLightModification = min;
		VoxelCordinates maxLightModification = max;
		// We want to add a 16 layer border
		for (int i = 0; i < 3; i++) {
			minLightModification[i] -= 16;
			maxLightModification[i] += 16;
		}

		m_worldProps.clampCordinates(minLightModification);
		m_worldProps.clampCordinates(maxLightModification);

		bakeAmbientLight(minLightModification.x, maxLightModification.x,
		                 minLightModification.z, maxLightModification.z);
		bakeVoxelLight(minLightModification, maxLightModification);
#endif
	}

	LayerVoxel VoxelWorld::readLayerVoxel(int x, int z) {
		LayerID layerID;
		LayerVoxelID layerVoxelID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);
		if (!m_worldProps.isValid(layerID)) return LayerVoxel();

		Layer& layer = m_layers[m_worldProps.getWorldLayerID(layerID)];
		return layer.readLayerVoxel(layerVoxelID);
	}

	LayerVoxel& VoxelWorld::modLayerVoxel(int x, int z) {
		LayerID layerID;
		LayerVoxelID layerVoxelID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);
		if (!m_worldProps.isValid(layerID)) return nullLayerVoxel;

		Layer& layer = m_layers[m_worldProps.getWorldLayerID(layerID)];
		return layer.modLayerVoxel(layerVoxelID);
	}

}  // namespace Deer
