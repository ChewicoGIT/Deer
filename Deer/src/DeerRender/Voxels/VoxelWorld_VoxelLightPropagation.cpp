#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Voxels/Chunk.h"

namespace Deer{
    void VoxelWorld::bakeVoxelLight(int minX, int maxX, int minY, int maxY, int minZ, int maxZ) {
        ChunkID minChunkID;
        ChunkID maxChunkID;
        ChunkVoxelID minChunkVoxelID;
        ChunkVoxelID maxChunkVoxelID;

        extractChunkCordinates(minX, minY, minZ, minChunkID, minChunkVoxelID);
        extractChunkCordinates(maxX, maxY, maxZ, maxChunkID, maxChunkVoxelID);

        tmp_voxelLightSource.clear();

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
                    workingChunk.clearVoxelLightAndSaveSources(workingMinVoxelID, workingMaxVoxelID, workingChunkID, tmp_voxelLightSource);
                }
            }
        }

        for (VoxelCordinates& cordinates : tmp_voxelLightSource) {
            VoxelLight& voxelLight = modLight(cordinates.x, cordinates.y, cordinates.z);
            Voxel voxel = readVoxel(cordinates.x, cordinates.y, cordinates.z);

            VoxelAspect& voxelAspect = VoxelData::voxelsAspect[voxel.id];
            voxelLight.r_light = voxelAspect.definition.colorEmission.r_value;
            voxelLight.g_light = voxelAspect.definition.colorEmission.g_value;
            voxelLight.b_light = voxelAspect.definition.colorEmission.b_value;

            m_voxelLightPropagation.push(cordinates);
        }

		while (!m_voxelLightPropagation.empty()) {
			resolveNextVoxelLightPropagation();
		}
    }

    void VoxelWorld::resolveNextVoxelLightPropagation() {
        VoxelCordinates position = m_voxelLightPropagation.front();
		m_voxelLightPropagation.pop();

		VoxelLight currentLight = readLight(position.x, position.y, position.z);
		bool solidCheck[6] = { false };
		// Check for every simple dir
		for (int i = 0; i < 6; i++) {
			int nextX = position.x + NORMAL_DIR(X_AXIS, i);
			int nextY = position.y + NORMAL_DIR(Y_AXIS, i);
			int nextZ = position.z + NORMAL_DIR(Z_AXIS, i);

			Voxel nextVoxel = readVoxel(nextX, nextY, nextZ);
			solidCheck[i] = nextVoxel.isVoxelType();
			if (solidCheck[i])
				continue;

			VoxelLight& nextLight = modLight(nextX, nextY, nextZ);
			int nextLightRedMinValue = currentLight.r_light - LIGHT_PROPAGATION_SIMPLE_FALL;
			int nextLightGreenMinValue = currentLight.g_light - LIGHT_PROPAGATION_SIMPLE_FALL;
			int nextLightBlueMinValue = currentLight.b_light - LIGHT_PROPAGATION_SIMPLE_FALL;

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
                m_voxelLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
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
                m_voxelLightPropagation.push(VoxelCordinates(nextX, nextY, nextZ));
		}
    }
}