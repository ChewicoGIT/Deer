#include "TerrainEditor.h"

#include "DeerStudio/Project.h"
#include "Deer/Scene.h"

#include "Deer/VoxelWorld.h"
#include "Deer/Voxel.h"


#include "DeerRender/Scene/SceneCamera.h"
#include "DeerRender/Render/Texture.h"

#include "DeerStudio/Editor/EditorUtils.h"
#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Editor/Icons.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

#include <string>

namespace Deer {
	namespace TerrainEditor {
		void deleteVoxelWorld();
	
		uint16_t selectedVoxelID = 1;
		TerrainEditMode terrainEditMode = TerrainEditMode_Add;
	}

	void TerrainEditor::terrainEditor_onImGui() {
		ImGui::Begin("Terrain Editor");

		Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
		if (voxelWorld == nullptr) {
			if (ImGui::Button("Create Voxel World")) {
				ImGui::OpenPopup(TERRAIN_EDITOR_CREATE_VOXEL_WORLD_POPUP_NAME);
			}

			createVoxelWorldPopup();
			ImGui::End();
			return;
		}

		ImGui::Text("Edit mode: ");

		setupColumns(ICON_BTN_MIN_SIZE + 16);
		if (iconButton((ImTextureID)(uint64_t)Icons::add_icon->getTextureID(), ICON_BTN_MIN_SIZE, terrainEditMode == TerrainEditMode_Add)) {
			terrainEditMode = TerrainEditMode_Add;
		}
		ImGui::NextColumn();
		if (iconButton((ImTextureID)(uint64_t)Icons::substract_icon->getTextureID(), ICON_BTN_MIN_SIZE, terrainEditMode == TerrainEditMode_Substract)) {
			terrainEditMode = TerrainEditMode_Substract;
		}
		ImGui::NextColumn();
		if (iconButton((ImTextureID)(uint64_t)Icons::box_select->getTextureID(), ICON_BTN_MIN_SIZE, terrainEditMode == TerrainEditMode_box_select)) {
			terrainEditMode = TerrainEditMode_box_select;
		}
		ImGui::Columns();
		ImGui::Separator();

		if (terrainEditMode == TerrainEditMode_Add) {
			voxelSelector();
		}

		if (terrainEditMode == TerrainEditMode_box_select){
			TerrainEditor::boxSelect();
		}

		if (ImGui::Button("Delete voxel world")) {
			ImGui::OpenPopup("DELETE_VOXEL_WORLD");
		}

		if (ImGui::Button("Create Ceiling")) {
			VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
			Project::m_scene.getVoxelWorld()->fillVoxels(
				0, 32 * worldProps.chunkSizeX - 1,
				0, 16,
				0, 32 * worldProps.chunkSizeZ - 1, 
				Voxel(VoxelData::getVoxelID("wood")));
			
			Project::m_scene.getVoxelWorld()->fillVoxels(
				1, 32 * worldProps.chunkSizeX - 2,
				8, 15,
				1, 32 * worldProps.chunkSizeZ - 2, 
				Voxel(VoxelData::getVoxelID("air")));

		}

		deleteInputPopup<deleteVoxelWorld>("DELETE_VOXEL_WORLD", "Are you sure you want to delete voxel world?");
		voxelRay();

		ImGui::End();
	}

	void TerrainEditor::deleteVoxelWorld() {
		Project::m_scene.deleteVoxelWorld();
	}
}