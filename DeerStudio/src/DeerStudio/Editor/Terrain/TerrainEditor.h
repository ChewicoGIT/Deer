#pragma once
#include "Deer/Core/Core.h"

#define TERRAIN_EDITOR_CREATE_VOXEL_WORLD_POPUP_NAME "CREATE_VOXEL_WORLD"

namespace Deer {
	enum TerrainEditMode {
		TerrainEditMode_Add = 0,
		TerrainEditMode_Substract = 1,
		TerrainEditMode_Fill = 2,
		TerrainEditMode_Empty = 3
	};

	namespace TerrainEditor {
		extern TerrainEditMode terrainEditMode;
		extern uint16_t selectedVoxelID;

		void terrainEditor_onImGui();
		void createVoxelWorldPopup();
		void voxelSelector();
		void voxelRay();
	}
}