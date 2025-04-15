#include "Deer/VoxelWorld.h"
#include "Deer/Voxels/Chunk.h"
#include "DeerRender/Voxels/VoxelWorldRenderData.h"

namespace Deer {
	void VoxelWorld::bakeVoxelLightFromPoint(VoxelCordinates coords) {
		VoxelCordinates min =
		    VoxelCordinates(coords.x - 16, coords.y - 16, coords.z - 16);
		VoxelCordinates max =
		    VoxelCordinates(coords.x + 16, coords.y + 16, coords.z + 16);

		m_worldProps.clampCordinates(min);
		m_worldProps.clampCordinates(max);

		bakeVoxelLight(min, max);
	}

	void VoxelWorld::bakeVoxelLight(VoxelCordinates min, VoxelCordinates max) {
		// We want to make the box 1 layer smaller
		// For every axis, X & Y & Z
		for (int i = 0; i < 3; i++) {
			min[i]++;
			max[i]--;
		}
		ChunkID minChunkID;
		ChunkID maxChunkID;
		ChunkVoxelID minChunkVoxelID;
		ChunkVoxelID maxChunkVoxelID;

		extractChunkCordinates(min, minChunkID, minChunkVoxelID);
		extractChunkCordinates(max, maxChunkID, maxChunkVoxelID);

		m_renderData->tmp_voxelLightSource.clear();

		// We want to empty the voxel light of the section first
		ChunkID workingChunkID;
		for (workingChunkID.x = minChunkID.x; workingChunkID.x <= maxChunkID.x;
		     workingChunkID.x++) {
			for (workingChunkID.y = minChunkID.y;
			     workingChunkID.y <= maxChunkID.y; workingChunkID.y++) {
				for (workingChunkID.z = minChunkID.z;
				     workingChunkID.z <= maxChunkID.z; workingChunkID.z++) {
					ChunkVoxelID workingMinVoxelID(
					    (workingChunkID.x == minChunkID.x) ? minChunkVoxelID.x
					                                       : 0,
					    (workingChunkID.y == minChunkID.y) ? minChunkVoxelID.y
					                                       : 0,
					    (workingChunkID.z == minChunkID.z) ? minChunkVoxelID.z
					                                       : 0);
					ChunkVoxelID workingMaxVoxelID(
					    (workingChunkID.x == maxChunkID.x) ? maxChunkVoxelID.x
					                                       : CHUNK_SIZE_X - 1,
					    (workingChunkID.y == maxChunkID.y) ? maxChunkVoxelID.y
					                                       : CHUNK_SIZE_Y - 1,
					    (workingChunkID.z == maxChunkID.z) ? maxChunkVoxelID.z
					                                       : CHUNK_SIZE_Z - 1);

					Chunk& workingChunk =
					    m_chunks[m_worldProps.getWorldChunkID(workingChunkID)];
					workingChunk.clearVoxelLightAndSaveSources(
					    workingMinVoxelID, workingMaxVoxelID, workingChunkID,
					    m_renderData->tmp_voxelLightSource);
				}
			}
		}

		// Cover all 6 edges with light propagation
		for (int x = min.x; x <= max.x; x++) {
			for (int y = min.y; y <= max.y; y++) {
				VoxelCordinates minZEdge(x, y, min.z - 1);
				VoxelCordinates maxZEdge(x, y, max.z + 1);

				VoxelLight minZEdgeLight = readLight(minZEdge);
				VoxelLight maxZEdgeLight = readLight(maxZEdge);

				if (minZEdgeLight.b_light || minZEdgeLight.g_light ||
				    minZEdgeLight.b_light)
					m_renderData->voxelLightPropagation.push(minZEdge);
				if (maxZEdgeLight.b_light || maxZEdgeLight.g_light ||
				    maxZEdgeLight.b_light)
					m_renderData->voxelLightPropagation.push(maxZEdge);
			}
		}

		for (int x = min.x; x <= max.x; x++) {
			for (int z = min.z; z <= max.z; z++) {
				VoxelCordinates minYEdge(x, min.y - 1, z);
				VoxelCordinates maxYEdge(x, max.y + 1, z);

				VoxelLight minYEdgeLight = readLight(minYEdge);
				VoxelLight maxYEdgeLight = readLight(maxYEdge);

				if (minYEdgeLight.b_light || minYEdgeLight.g_light ||
				    minYEdgeLight.b_light)
					m_renderData->voxelLightPropagation.push(minYEdge);
				if (maxYEdgeLight.b_light || maxYEdgeLight.g_light ||
				    maxYEdgeLight.b_light)
					m_renderData->voxelLightPropagation.push(maxYEdge);
			}
		}

		for (int y = min.y; y <= max.y; y++) {
			for (int z = min.z; z <= max.z; z++) {
				VoxelCordinates minXEdge(min.x - 1, y, z);
				VoxelCordinates maxXEdge(max.x + 1, y, z);

				VoxelLight minXEdgeLight = readLight(minXEdge);
				VoxelLight maxXEdgeLight = readLight(maxXEdge);

				if (minXEdgeLight.b_light || minXEdgeLight.g_light ||
				    minXEdgeLight.b_light)
					m_renderData->voxelLightPropagation.push(minXEdge);
				if (maxXEdgeLight.b_light || maxXEdgeLight.g_light ||
				    maxXEdgeLight.b_light)
					m_renderData->voxelLightPropagation.push(maxXEdge);
			}
		}

		for (VoxelCordinates& cordinates : m_renderData->tmp_voxelLightSource) {
			VoxelLight& voxelLight = modLight(cordinates);
			Voxel voxel = readVoxel(cordinates);

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

		VoxelLight currentLight = readLight(position);
		bool voxelCheck[6] = {false};

		int highestRGBValue = currentLight.r_light;
		if (highestRGBValue < currentLight.g_light)
			highestRGBValue = currentLight.g_light;
		if (highestRGBValue < currentLight.b_light)
			highestRGBValue = currentLight.b_light;

		int proportionalDecrease =
		    (LIGHT_PROPAGATION_SIMPLE_FALL * 100) / highestRGBValue;

		int nextLightRedMinValue =
		    currentLight.r_light -
		    (proportionalDecrease * currentLight.r_light) / 100;
		int nextLightGreenMinValue =
		    currentLight.g_light -
		    (proportionalDecrease * currentLight.g_light) / 100;
		int nextLightBlueMinValue =
		    currentLight.b_light -
		    (proportionalDecrease * currentLight.b_light) / 100;

		// Check for every simple dir
		for (int i = 0; i < 6; i++) {
			VoxelCordinates next(position.x + NORMAL_DIR(X_AXIS, i),
			                     position.y + NORMAL_DIR(Y_AXIS, i),
			                     position.z + NORMAL_DIR(Z_AXIS, i));

			Voxel nextVoxel = readVoxel(next);
			voxelCheck[i] = nextVoxel.isVoxelType();
			if (voxelCheck[i]) continue;

			VoxelLight& nextLight = modLight(next);

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
				m_renderData->voxelLightPropagation.push(next);
		}

		return;
		// TODO
		// Check for every complex dir
		for (int i = 0; i < 6; i++) {
			int cDir0 = LIGHT_PROPAGATION_COMPLEX_DIR(0, i);
			int cDir1 = LIGHT_PROPAGATION_COMPLEX_DIR(1, i);

			if (voxelCheck[cDir0] || voxelCheck[cDir1]) continue;

			VoxelCordinates next(position.x + NORMAL_DIR(X_AXIS, cDir0) +
			                         NORMAL_DIR(X_AXIS, cDir1),
			                     position.y + NORMAL_DIR(Y_AXIS, cDir0) +
			                         NORMAL_DIR(Y_AXIS, cDir1),
			                     position.z + NORMAL_DIR(Z_AXIS, cDir0) +
			                         NORMAL_DIR(Z_AXIS, cDir1));

			Voxel nextVoxel = readVoxel(next);
			if (nextVoxel.isVoxelType()) continue;

			VoxelLight& nextLight = modLight(next);
			int nextLightRedMinValue =
			    currentLight.r_light - LIGHT_PROPAGATION_COMPLEX_FALL;
			int nextLightGreenMinValue =
			    currentLight.g_light - LIGHT_PROPAGATION_COMPLEX_FALL;
			int nextLightBlueMinValue =
			    currentLight.b_light - LIGHT_PROPAGATION_COMPLEX_FALL;

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
				m_renderData->voxelLightPropagation.push(next);
		}
	}
}  // namespace Deer