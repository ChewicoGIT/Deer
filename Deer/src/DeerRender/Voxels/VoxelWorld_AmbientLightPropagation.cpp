#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Core/Application.h"
#include "Deer/Core/Project.h"
#include "Deer/Asset/AssetManager.h"
#include "Deer/Scene/Entity.h"
#include "Deer/Scene/Components.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "DeerRender/Render/Texture.h"

#include "Deer/Core/Log.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

namespace Deer {
	void VoxelWorld::bakeAmbientLight(int minX, int maxX, int minZ, int maxZ) {
		int xPos = minX;
		for (; xPos <= maxX; xPos++) {
			int zPos = minZ;
			for (; zPos <= maxZ; zPos++) {
				LayerVoxel layer = readLayerVoxel(xPos, zPos);
				uint16_t maxHeight = layer.height;
				uint16_t minHeight = layer.ambient_light_height;

				uint16_t maxHeightSorroundings = maxHeight;
				for (int i = 0; i < 8; i++) {
					int checkX = xPos + LAYER_CHECK_DIRS(X_AXIS, i);
					int checkZ = zPos + LAYER_CHECK_DIRS(Y_AXIS, i);

					LayerVoxel layer = readLayerVoxel(checkX, checkZ);
					if (layer.height > maxHeightSorroundings)
						maxHeightSorroundings = layer.height;
				}

				maxHeightSorroundings++;
				if (maxHeightSorroundings >= CHUNK_SIZE_Y * m_worldProps.chunkSizeY)
					maxHeightSorroundings = CHUNK_SIZE_Y * m_worldProps.chunkSizeY - 1;

				ChunkID chunkID;
				ChunkVoxelID chunkVoxelID;

				int yPos = minHeight;
				// All light blocks under the max height must be put to 0 ambient light and above to 255
				for (; yPos < maxHeight; yPos++) {
					extractChunkCordinates(xPos, yPos, zPos, chunkID, chunkVoxelID);
					Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
					VoxelLight& voxelLight = chunk.modLight(chunkVoxelID);

					voxelLight.ambient_light = 0;
				}

				// Fill with light blocks and add queue to update light propagation
				for (; yPos < maxHeightSorroundings; yPos++) {
					extractChunkCordinates(xPos, yPos, zPos, chunkID, chunkVoxelID);
					Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
					VoxelLight& voxelLight = chunk.modLight(chunkVoxelID);

					voxelLight.ambient_light = 255;
					m_ambientLightPropagation.push(VoxelCordinates(xPos, yPos, zPos));
				}

				yPos++;
				if (yPos < CHUNK_SIZE_Y * m_worldProps.chunkSizeY) {
					extractChunkCordinates(xPos, yPos, zPos, chunkID, chunkVoxelID);
					Chunk& chunk = m_chunks[m_worldProps.getWorldChunkID(chunkID)];
					VoxelLight& voxelLight = chunk.modLight(chunkVoxelID);

					voxelLight.ambient_light = 255;
				}

				modLayerVoxel(xPos, zPos).ambient_light_height = CHUNK_SIZE_Y * m_worldProps.chunkSizeY;
			}
		}

		//Resolve ambient light propagation
		while (!m_ambientLightPropagation.empty()) {
			resolveNextAmbientLightPropagation();
		}
	}

	void VoxelWorld::bakeAmbientLightFromPoint(int x, int z) {
		int minX = x - 16;
		int maxX = x + 16;

		int minZ = z - 16;
		int maxZ = z + 16;

		minX = (minX < 0) ? 0 : minX;
		maxX = (maxX >= m_worldProps.chunkSizeX * CHUNK_SIZE_X) ? m_worldProps.chunkSizeX * CHUNK_SIZE_X - 1 : maxX;

		minZ = (minZ < 0) ? 0 : minZ;
		maxZ = (maxZ >= m_worldProps.chunkSizeZ * CHUNK_SIZE_Z) ? m_worldProps.chunkSizeZ * CHUNK_SIZE_Z - 1 : maxZ;

		bakeAmbientLight(minX, maxX, minZ, maxZ);
	}

	void VoxelWorld::resolveNextAmbientLightPropagation() {
		VoxelCordinates position = m_ambientLightPropagation.front();
		m_ambientLightPropagation.pop();

		LayerVoxel& layerVoxel = modLayerVoxel(position.x, position.z);
		if (layerVoxel.ambient_light_height > position.y)
			layerVoxel.ambient_light_height = position.y;

		VoxelLight currentLight = readLight(position.x, position.y, position.z);
		bool solidCheck[6] = { false };
		// Check for every simple dir
		for (int i = 0; i < 6; i++) {
			int nextX = position.x + NORMAL_DIR(X_AXIS, i);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, i);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, i);

			//DEER_CORE_ASSERT(nextX < 32, "well");

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			solidCheck[i] = nextVoxel.id != 0;
			if (nextVoxel.id != 0)
				continue;

			VoxelLight nextLight = readLight(nextX, nextY, nextZ);
			int nextLightMinValue = currentLight.ambient_light - LIGHT_PROPAGATION_SIMPLE_FALL;

			if (nextLight.ambient_light < nextLightMinValue) {
				modLight(nextX, nextY, nextZ).ambient_light = nextLightMinValue;
				m_ambientLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
			}
		}

		// Check for every complex dir
		for (int i = 0; i < 6; i++) {
			int cDir0 = LIGHT_PROPAGATION_COMPLEX_DIR(0, i);
			int cDir1 = LIGHT_PROPAGATION_COMPLEX_DIR(1, i);

			if (solidCheck[cDir0] || solidCheck[cDir1])
				continue;

			int nextX = position.x + NORMAL_DIR(X_AXIS, cDir0) + NORMAL_DIR(X_AXIS, cDir1);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, cDir0) + NORMAL_DIR(Y_AXIS, cDir1);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, cDir0) + NORMAL_DIR(Z_AXIS, cDir1);

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			solidCheck[i] = nextVoxel.id != 0;
			if (nextVoxel.id != 0)
				continue;

			VoxelLight nextLight = readLight(nextX, nextY, nextZ);
			int nextLightMinValue = currentLight.ambient_light - LIGHT_PROPAGATION_COMPLEX_FALL;

			if (nextLight.ambient_light < nextLightMinValue) {
				modLight(nextX, nextY, nextZ).ambient_light = nextLightMinValue;
				m_ambientLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
			}
		}
	}

}