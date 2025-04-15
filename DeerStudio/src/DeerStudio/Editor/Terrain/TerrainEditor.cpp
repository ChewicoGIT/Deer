#include "TerrainEditor.h"

#include <string>

#include "Deer/Scene.h"
#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"
#include "DeerRender/Render/Texture.h"
#include "DeerRender/SceneCamera.h"
#include "DeerStudio/Editor/EditorUtils.h"
#include "DeerStudio/Editor/Icons.h"
#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Project.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

namespace Deer {
	namespace TerrainEditor {
		uint16_t selectedVoxelID = 1;
		TerrainEditMode terrainEditMode = TerrainEditMode_Add;
	}  // namespace TerrainEditor

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
		if (iconButton((ImTextureID)(uint64_t)Icons::info_icon->getTextureID(),
		               ICON_BTN_MIN_SIZE,
		               terrainEditMode == TerrainEditMode_Info)) {
			terrainEditMode = TerrainEditMode_Info;
		}
		ImGui::NextColumn();
		if (iconButton((ImTextureID)(uint64_t)Icons::add_icon->getTextureID(),
		               ICON_BTN_MIN_SIZE,
		               terrainEditMode == TerrainEditMode_Add)) {
			terrainEditMode = TerrainEditMode_Add;
		}
		ImGui::NextColumn();
		if (iconButton(
		        (ImTextureID)(uint64_t)Icons::substract_icon->getTextureID(),
		        ICON_BTN_MIN_SIZE,
		        terrainEditMode == TerrainEditMode_Substract)) {
			terrainEditMode = TerrainEditMode_Substract;
		}
		ImGui::NextColumn();
		if (iconButton((ImTextureID)(uint64_t)Icons::fill_icon->getTextureID(),
		               ICON_BTN_MIN_SIZE,
		               terrainEditMode == TerrainEditMode_fill)) {
			terrainEditMode = TerrainEditMode_fill;
		}
		ImGui::NextColumn();
		if (iconButton(
		        (ImTextureID)(uint64_t)Icons::fill_empty_icon->getTextureID(),
		        ICON_BTN_MIN_SIZE,
		        terrainEditMode == TerrainEditMode_empty_fill)) {
			terrainEditMode = TerrainEditMode_empty_fill;
		}
		ImGui::NextColumn();
		if (iconButton(
		        (ImTextureID)(uint64_t)Icons::box_select_icon->getTextureID(),
		        ICON_BTN_MIN_SIZE,
		        terrainEditMode == TerrainEditMode_box_select)) {
			terrainEditMode = TerrainEditMode_box_select;
		}
		ImGui::Columns();
		ImGui::Separator();

		switch (terrainEditMode) {
			case TerrainEditMode_Info:
				info();
				break;

			case TerrainEditMode_Add:
				voxelSelector();
				break;

			case TerrainEditMode_Substract:

				break;

			case TerrainEditMode_fill:
				fill();
				break;

			case TerrainEditMode_empty_fill:
				empty();
				break;

			case TerrainEditMode_box_select:
				TerrainEditor::boxSelect();
				break;
		}

		if (terrainEditMode == TerrainEditMode_Info) {
		} else {
			voxelRay();
			boxSelect_Visuals();
		}
		ImGui::End();
	}

}  // namespace Deer