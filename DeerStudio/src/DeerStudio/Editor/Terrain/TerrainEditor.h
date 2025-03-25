#pragma once
#include "Deer/Voxel.h"

#define TERRAIN_EDITOR_CREATE_VOXEL_WORLD_POPUP_NAME "CREATE_VOXEL_WORLD"
#define FACE_VOXEL_SELECT 0
#define INTERNAL_VOXEL_SELECT 1

namespace Deer {
	enum TerrainEditMode {
		TerrainEditMode_Info = 0,
		TerrainEditMode_Add = 1,
		TerrainEditMode_Substract = 2,
		TerrainEditMode_fill = 3,
		TerrainEditMode_empty_fill = 4,
		TerrainEditMode_box_select = 5
	};

	namespace TerrainEditor {
		extern TerrainEditMode terrainEditMode;
		extern uint16_t selectedVoxelID;
		extern VoxelCordinates voxelRayCoords;
		extern VoxelCordinates voxelFaceRayCoords;
		extern uint8_t voxelSelectMode; 

		void terrainEditor_onImGui();
		void createVoxelWorldPopup();
		void voxelSelector();
		void voxelRay();
		void boxSelect();
		void boxSelect_Visuals();
		void fill();
		void info();
	}
}