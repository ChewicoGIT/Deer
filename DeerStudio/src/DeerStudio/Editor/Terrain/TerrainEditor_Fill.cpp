#include "Deer/Scene.h"
#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"
#include "DeerRender/GizmoRenderer.h"
#include "DeerStudio/Editor/EditorUtils.h"
#include "DeerStudio/Editor/Icons.h"
#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Project.h"
#include "TerrainEditor.h"
#include "imgui.h"

namespace Deer {
	void TerrainEditor::fill() {
		ImGui::Text("Select mode: ");
		ImGui::SameLine();
		if (voxelSelectMode == FACE_VOXEL_SELECT)
			ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "%s", "Face");
		else
			ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "%s", "Voxel");

		setupColumns(ICON_BTN_MIN_SIZE + 16);
		if (iconButton((ImTextureID)(uint64_t)
		                   Icons::face_voxel_selection_icon->getTextureID(),
		               ICON_BTN_MIN_SIZE,
		               voxelSelectMode == FACE_VOXEL_SELECT)) {
			voxelSelectMode = FACE_VOXEL_SELECT;
		}
		ImGui::Text("Face");
		ImGui::NextColumn();
		if (iconButton((ImTextureID)(uint64_t)
		                   Icons::internal_voxel_selection_icon->getTextureID(),
		               ICON_BTN_MIN_SIZE,
		               voxelSelectMode == INTERNAL_VOXEL_SELECT)) {
			voxelSelectMode = INTERNAL_VOXEL_SELECT;
		}
		ImGui::Text("Voxel");
		ImGui::Columns();

		ImGui::Separator();
		ImGui::Spacing();

		voxelSelector();

		if (!viewportIsActive()) return;

		VoxelCordinates selectVoxel;
		VoxelCordinates clampedCordinates;
		if (voxelSelectMode == FACE_VOXEL_SELECT) {
			selectVoxel = voxelFaceRayCoords;
		} else {
			selectVoxel = voxelRayCoords;
		}

		clampedCordinates = selectVoxel;
		Project::m_scene.getVoxelWorld()->getVoxelWorldProps().clampCordinates(
		    clampedCordinates);

		if (clampedCordinates != selectVoxel) return;

		VoxelCordinates min = selectedVoxelStart;
		VoxelCordinates max = selectedVoxelEnd;

		if (min.isNull() || max.isNull()) {
			min = VoxelCordinates(selectVoxel.x - 16, selectVoxel.y - 16,
			                      selectVoxel.z - 16);
			max = VoxelCordinates(selectVoxel.x + 15, selectVoxel.y + 15,
			                      selectVoxel.z + 15);
		}
		Project::m_scene.getVoxelWorld()
		    ->getVoxelWorldProps()
		    .clampAndSetMinMax(min, max);

		if (selectVoxel.x < min.x || selectVoxel.x > max.x ||
		    selectVoxel.y < min.y || selectVoxel.y > max.y ||
		    selectVoxel.z < min.z || selectVoxel.z > max.z)
			return;

		for (int i = 0; i < 6; i++) {
			Project::m_scene.getMainGizmoRenderer().drawVoxelFace(
			    clampedCordinates.x, clampedCordinates.y, clampedCordinates.z,
			    selectedVoxelID, i, 0);
		}

		if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left)) {
			Voxel voxel =
			    Project::m_scene.getVoxelWorld()->readVoxel(selectVoxel);
			Project::m_scene.getVoxelWorld()->remplaceVoxels(
			    min, max, voxel, Voxel(selectedVoxelID));
		}
	}
}  // namespace Deer