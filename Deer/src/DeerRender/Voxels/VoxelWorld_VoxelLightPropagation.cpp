#include "Deer/VoxelWorld.h"
#include "Deer/Voxels/Chunk.h"
#include "DeerRender/Voxels/VoxelWorldRenderData.h"

namespace Deer{
    void VoxelWorld::bakeVoxelLightFromPoint(int x, int y, int z) {
        int min[3] {x - 16, y - 16, z - 16};
        int max[3] {x + 16, y + 16, z + 16};

        for (int i = 0; i < 3; i++){
            min[i] = (min[i] < 0) ? 0 : min[i];
        }

        max[0] = (max[0] >= CHUNK_SIZE_X * m_worldProps.chunkSizeX) ? CHUNK_SIZE_X * m_worldProps.chunkSizeX - 1 : max[0];
        max[1] = (max[1] >= CHUNK_SIZE_Y * m_worldProps.chunkSizeY) ? CHUNK_SIZE_Y * m_worldProps.chunkSizeY - 1 : max[1];
        max[2] = (max[2] >= CHUNK_SIZE_Z * m_worldProps.chunkSizeZ) ? CHUNK_SIZE_Z * m_worldProps.chunkSizeZ - 1 : max[2];

        bakeVoxelLight(min[0], max[0], min[1], max[1], min[2], max[2]);
    }

    void VoxelWorld::bakeVoxelLight(int minX, int maxX, int minY, int maxY, int minZ, int maxZ) {
        ChunkID minChunkID;
        ChunkID maxChunkID;
        ChunkVoxelID minChunkVoxelID;
        ChunkVoxelID maxChunkVoxelID;

        extractChunkCordinates(minX + 1, minY + 1, minZ + 1, minChunkID, minChunkVoxelID);
        extractChunkCordinates(maxX - 1, maxY - 1, maxZ - 1, maxChunkID, maxChunkVoxelID);

        m_renderData->tmp_voxelLightSource.clear();

        // We want to empty the voxel light of the section first
        ChunkID workingChunkID;
        for (workingChunkID.x = minChunkID.x; workingChunkID.x <= maxChunkID.x; workingChunkID.x++) {
            for (workingChunkID.y = minChunkID.y; workingChunkID.y <= maxChunkID.y; workingChunkID.y++) {
                for (workingChunkID.z = minChunkID.z; workingChunkID.z <= maxChunkID.z; workingChunkID.z++) {
                    ChunkVoxelID workingMinVoxelID(
                        (workingChunkID.x == minChunkID.x)? minChunkVoxelID.x : 0, 
                        (workingChunkID.y == minChunkID.y)? minChunkVoxelID.y : 0, 
                        (workingChunkID.z == minChunkID.z)? minChunkVoxelID.z : 0);
                    ChunkVoxelID workingMaxVoxelID(
                        (workingChunkID.x == maxChunkID.x)? maxChunkVoxelID.x : CHUNK_SIZE_X - 1, 
                        (workingChunkID.y == maxChunkID.y)? maxChunkVoxelID.y : CHUNK_SIZE_Y - 1, 
                        (workingChunkID.z == maxChunkID.z)? maxChunkVoxelID.z : CHUNK_SIZE_Z - 1);

                    Chunk& workingChunk = m_chunks[m_worldProps.getWorldChunkID(workingChunkID)];
                    workingChunk.clearVoxelLightAndSaveSources(workingMinVoxelID, workingMaxVoxelID, workingChunkID, m_renderData->tmp_voxelLightSource);
                }
            }
        }

        // Cover all 6 edges with light propagation
        for (int x = minX; x <= maxX; x++) {
            for (int y = minY; y <= maxY; y++) {
                VoxelCordinates minZEdge(x, y, minZ);
                VoxelCordinates maxZEdge(x, y, maxZ);

                VoxelLight minZEdgeLight = readLight(minZEdge.x, minZEdge.y, minZEdge.z);
                VoxelLight maxZEdgeLight = readLight(maxZEdge.x, maxZEdge.y, maxZEdge.z);

                if (minZEdgeLight.b_light || minZEdgeLight.g_light || minZEdgeLight.b_light)
                    m_renderData->voxelLightPropagation.push(minZEdge);
                if (maxZEdgeLight.b_light || maxZEdgeLight.g_light || maxZEdgeLight.b_light)
                    m_renderData->voxelLightPropagation.push(maxZEdge);
            }
        }

        for (int x = minX; x <= maxX; x++) {
            for (int z = minZ; z <= maxZ; z++) {
                VoxelCordinates minYEdge(x, minY, z);
                VoxelCordinates maxYEdge(x, maxY, z);

                VoxelLight minYEdgeLight = readLight(minYEdge.x, minYEdge.y, minYEdge.z);
                VoxelLight maxYEdgeLight = readLight(maxYEdge.x, maxYEdge.y, maxYEdge.z);

                if (minYEdgeLight.b_light || minYEdgeLight.g_light || minYEdgeLight.b_light)
                    m_renderData->voxelLightPropagation.push(minYEdge);
                if (maxYEdgeLight.b_light || maxYEdgeLight.g_light || maxYEdgeLight.b_light)
                    m_renderData->voxelLightPropagation.push(maxYEdge);
            }
        }

        for (int y = minY; y <= maxY; y++) {
            for (int z = minZ; z <= maxZ; z++) {
                VoxelCordinates minXEdge(minX, y, z);
                VoxelCordinates maxXEdge(maxX, y, z);

                VoxelLight minXEdgeLight = readLight(minXEdge.x, minXEdge.y, minXEdge.z);
                VoxelLight maxXEdgeLight = readLight(maxXEdge.x, maxXEdge.y, maxXEdge.z);

                if (minXEdgeLight.b_light || minXEdgeLight.g_light || minXEdgeLight.b_light)
                    m_renderData->voxelLightPropagation.push(minXEdge);
                if (maxXEdgeLight.b_light || maxXEdgeLight.g_light || maxXEdgeLight.b_light)
                    m_renderData->voxelLightPropagation.push(maxXEdge);
            }
        }

        for (VoxelCordinates& cordinates : m_renderData->tmp_voxelLightSource) {
            VoxelLight& voxelLight = modLight(cordinates.x, cordinates.y, cordinates.z);
            Voxel voxel = readVoxel(cordinates.x, cordinates.y, cordinates.z);

            VoxelAspect& voxelAspect = VoxelData::voxelsAspect[voxel.id];
            voxelLight.r_light = voxelAspect.definition.colorEmission.r_value;
            voxelLight.g_light = voxelAspect.definition.colorEmission.g_value;
            voxelLight.b_light = voxelAspect.definition.colorEmission.b_value;

            m_renderData->voxelLightPropagation.push(cordinates);
        }

		while (!m_renderData->voxelLightPropagation.empty()) {
			resolveNextVoxelLightPropagation();
		}
    }

    void VoxelWorld::resolveNextVoxelLightPropagation() {
        VoxelCordinates position = m_renderData->voxelLightPropagation.front();
		m_renderData->voxelLightPropagation.pop();

		VoxelLight currentLight = readLight(position.x, position.y, position.z);
		bool voxelCheck[6] = { false };

        int highestRGBValue = currentLight.r_light;
        if (highestRGBValue < currentLight.g_light)
            highestRGBValue = currentLight.g_light;
        if (highestRGBValue < currentLight.b_light)
            highestRGBValue = currentLight.b_light;

        int proportionalDecrease = (LIGHT_PROPAGATION_SIMPLE_FALL * 100) / highestRGBValue;

        int nextLightRedMinValue = currentLight.r_light - (proportionalDecrease * currentLight.r_light) / 100;
        int nextLightGreenMinValue = currentLight.g_light - (proportionalDecrease * currentLight.g_light) / 100;
        int nextLightBlueMinValue = currentLight.b_light - (proportionalDecrease * currentLight.b_light) / 100;

		// Check for every simple dir
		for (int i = 0; i < 6; i++) {
			int nextX = position.x + NORMAL_DIR(X_AXIS, i);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, i);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, i);

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			voxelCheck[i] = nextVoxel.isVoxelType();
			if (voxelCheck[i])
				continue;

			VoxelLight& nextLight = modLight(nextX, nextY, nextZ);

            bool nextVoxelModified = false;
			if (nextLight.r_light < nextLightRedMinValue) {
				nextLight.r_light = nextLightRedMinValue;
                nextVoxelModified = true;
			}

            if (nextLight.g_light < nextLightGreenMinValue) {
				nextLight.g_light = nextLightGreenMinValue;
                nextVoxelModified = true;
			}

            if (nextLight.b_light < nextLightBlueMinValue) {
				nextLight.b_light = nextLightBlueMinValue;
                nextVoxelModified = true;
			}

            if (nextVoxelModified)
                m_renderData->voxelLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
		}

		return;
		// TODO
		// Check for every complex dir
		for (int i = 0; i < 6; i++) {
			int cDir0 = LIGHT_PROPAGATION_COMPLEX_DIR(0, i);
			int cDir1 = LIGHT_PROPAGATION_COMPLEX_DIR(1, i);

			if (voxelCheck[cDir0] || voxelCheck[cDir1])
				continue;

			int nextX = position.x + NORMAL_DIR(X_AXIS, cDir0) + NORMAL_DIR(X_AXIS, cDir1);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, cDir0) + NORMAL_DIR(Y_AXIS, cDir1);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, cDir0) + NORMAL_DIR(Z_AXIS, cDir1);

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			if (nextVoxel.isVoxelType())
				continue;

			VoxelLight& nextLight = modLight(nextX, nextY, nextZ);
			int nextLightRedMinValue = currentLight.r_light - LIGHT_PROPAGATION_COMPLEX_FALL;
			int nextLightGreenMinValue = currentLight.g_light - LIGHT_PROPAGATION_COMPLEX_FALL;
			int nextLightBlueMinValue = currentLight.b_light - LIGHT_PROPAGATION_COMPLEX_FALL;

            bool nextVoxelModified = false;
			if (nextLight.r_light < nextLightRedMinValue) {
				nextLight.r_light = nextLightRedMinValue;
                nextVoxelModified = true;
			}

            if (nextLight.g_light < nextLightGreenMinValue) {
				nextLight.g_light = nextLightGreenMinValue;
                nextVoxelModified = true;
			}

            if (nextLight.b_light < nextLightBlueMinValue) {
				nextLight.b_light = nextLightBlueMinValue;
                nextVoxelModified = true;
			}

            if (nextVoxelModified)
                m_renderData->voxelLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
		}
    }
}