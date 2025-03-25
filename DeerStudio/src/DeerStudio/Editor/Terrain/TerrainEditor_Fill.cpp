#include "TerrainEditor.h"
#include "DeerStudio/Editor/Icons.h"
#include "DeerStudio/Editor/EditorUtils.h"

#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"

#include "DeerRender/GizmoRenderer.h"

#include "imgui.h"

namespace Deer {
    void TerrainEditor::fill() {
        ImGui::Text("Select mode: ");
        ImGui::SameLine();
        if (voxelSelectMode == FACE_VOXEL_SELECT)
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "%s","Face");
        else
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "%s","Voxel");

        setupColumns(ICON_BTN_MIN_SIZE + 16);
        if (iconButton((ImTextureID)(uint64_t)Icons::face_voxel_selection_icon->getTextureID(), ICON_BTN_MIN_SIZE, voxelSelectMode == FACE_VOXEL_SELECT)) {
            voxelSelectMode = FACE_VOXEL_SELECT;
        }
        ImGui::Text("Face");
        ImGui::NextColumn();
        if (iconButton((ImTextureID)(uint64_t)Icons::internal_voxel_selection_icon->getTextureID(), ICON_BTN_MIN_SIZE, voxelSelectMode == INTERNAL_VOXEL_SELECT)) {
            voxelSelectMode = INTERNAL_VOXEL_SELECT;
        }
        ImGui::Text("Voxel");
        ImGui::Columns();

        ImGui::Separator();
        ImGui::Spacing();
        
		voxelSelector();

    }
}