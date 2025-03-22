#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Voxel.h"

#define TERRAIN_EDITOR_CREATE_VOXEL_WORLD_POPUP_NAME "CREATE_VOXEL_WORLD"

namespace Deer {
	enum TerrainEditMode {
		TerrainEditMode_Add = 0,
		TerrainEditMode_Substract = 1,
		TerrainEditMode_box_select = 2
	};

	namespace TerrainEditor {
		extern TerrainEditMode terrainEditMode;
		extern uint16_t selectedVoxelID;
		extern VoxelCordinates voxelRayCoords;
		extern VoxelCordinates voxelFaceRayCoords;

		void terrainEditor_onImGui();
		void createVoxelWorldPopup();
		void voxelSelector();
		void voxelRay();
		void boxSelect();
	}
}