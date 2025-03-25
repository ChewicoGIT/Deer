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
	VoxelWorld::VoxelWorld(const VoxelWorldProps& props) 
		: m_worldProps(props) {
		m_chunks = MakeScope<Chunk[]>(m_worldProps.getChunkCount());
		m_layers = MakeScope<Layer[]>(m_worldProps.getLayerCount());
#ifdef DEER_RENDER
		m_renderData = MakeScope<VoxelWorldRenderData>(m_worldProps.getChunkCount());
#endif
	}
	
	uint16_t VoxelWorld::calculateLayerVoxelHeight(int x, int z) {
		LayerVoxelID layerVoxelID;
		LayerID layerID;

		extractLayerCordinates(x, z, layerID, layerVoxelID);
		ChunkID chunkID(layerID.x, 0, layerID.z);

		for (int y = m_worldProps.chunkSizeY - 1; y >= 0; y--) {
			chunkID.y = y;

			Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
			uint8_t chunkVoxelHeight = chunk.calculateLayerVoxelHeight(layerVoxelID);

			if (chunkVoxelHeight != 0) {
				return chunkVoxelHeight + chunkID.y * CHUNK_SIZE_Y;
			}
		}
		return 0;
	}
}
