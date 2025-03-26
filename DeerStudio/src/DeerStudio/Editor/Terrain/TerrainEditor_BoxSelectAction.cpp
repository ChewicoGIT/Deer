#include "TerrainEditor.h"
#include "DeerStudio/Project.h"
#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Editor/EditorUtils.h"
#include "DeerStudio/Editor/Icons.h"

#include "Deer/Log.h"
#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"
#include "Deer/Scene.h"

#include "DeerRender/GizmoRenderer.h"
#include "DeerRender/Render/Texture.h"

#include "imgui.h"
namespace Deer {
    namespace TerrainEditor {
        VoxelCordinates selectedVoxelStart(-1, -1, -1);
        VoxelCordinates selectedVoxelEnd(-1, -1, -1);
        uint8_t voxelSelectMode = 0;
    }

    void TerrainEditor::boxSelect() {
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Select mode: ");
        ImGui::SameLine();
        if (voxelSelectMode == FACE_VOXEL_SELECT)
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "%s","Face");
        else
            ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.6f, 1.0f), "%s","Voxel");
        
        ImGui::Separator();
        ImGui::Spacing();
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

        if (!viewportIsActive()) {
            if (selectedVoxelStart.isNull())
                selectedVoxelStart.makeNull();
            return;
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            if (voxelSelectMode == FACE_VOXEL_SELECT) {
                selectedVoxelEnd = voxelFaceRayCoords;
            } else {
                selectedVoxelEnd = voxelRayCoords;
            }
            Project::m_scene.getVoxelWorld()->getVoxelWorldProps().clampCordinates(selectedVoxelEnd);
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) == 1) {
            selectedVoxelStart = selectedVoxelEnd;
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle) == 1) {
            selectedVoxelStart.makeNull();
            selectedVoxelEnd.makeNull();
        }

    }

    void TerrainEditor::boxSelect_Visuals() {

        Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
        GizmoRenderer& gizmo = Project::m_scene.getMainGizmoRenderer();

        if (!selectedVoxelStart.isNull() && !selectedVoxelEnd.isNull()) {
            VoxelCordinates min;
            VoxelCordinates max;

            if (selectedVoxelEnd.x < selectedVoxelStart.x) {
                min.x = selectedVoxelEnd.x;
                max.x = selectedVoxelStart.x;
            } else {
                max.x = selectedVoxelEnd.x;
                min.x = selectedVoxelStart.x;
            }

            if (selectedVoxelEnd.y < selectedVoxelStart.y) {
                min.y = selectedVoxelEnd.y;
                max.y = selectedVoxelStart.y;
            } else {
                max.y = selectedVoxelEnd.y;
                min.y = selectedVoxelStart.y;
            }

            if (selectedVoxelEnd.z < selectedVoxelStart.z) {
                min.z = selectedVoxelEnd.z;
                max.z = selectedVoxelStart.z;
            } else {
                max.z = selectedVoxelEnd.z;
                min.z = selectedVoxelStart.z;
            }

            int debugVoxel = VoxelData::getVoxelID("debug");
            int debugInternal = VoxelData::getVoxelID("debug_internal");

            // External faces
            // Z Face
            for (int x = min.x; x <= max.x; x++) {
                for (int y = min.y; y <= max.y; y++) {
                    gizmo.drawVoxelFace(x, y, min.z, debugVoxel, NORMAL_BACK);
                    gizmo.drawVoxelFace(x, y, max.z, debugVoxel, NORMAL_FRONT);
                    gizmo.drawVoxelFaceInternal(x, y, min.z, debugInternal, NORMAL_BACK);
                    gizmo.drawVoxelFaceInternal(x, y, max.z, debugInternal, NORMAL_FRONT);
                }
            }

            // Y Face
            for (int x = min.x; x <= max.x; x++) {
                for (int z = min.z; z <= max.z; z++) {
                    gizmo.drawVoxelFace(x, min.y, z, debugVoxel, NORMAL_DOWN);
                    gizmo.drawVoxelFace(x, max.y, z, debugVoxel, NORMAL_UP);
                    gizmo.drawVoxelFaceInternal(x, min.y, z, debugInternal, NORMAL_DOWN);
                    gizmo.drawVoxelFaceInternal(x, max.y, z, debugInternal, NORMAL_UP);
                }
            }

            // X Face
            for (int y = min.y; y <= max.y; y++) {
                for (int z = min.z; z <= max.z; z++) {
                    gizmo.drawVoxelFace(min.x, y, z, debugVoxel, NORMAL_LEFT);
                    gizmo.drawVoxelFace(max.x, y, z, debugVoxel, NORMAL_RIGHT);
                    gizmo.drawVoxelFaceInternal(min.x, y, z, debugInternal, NORMAL_LEFT);
                    gizmo.drawVoxelFaceInternal(max.x, y, z, debugInternal, NORMAL_RIGHT);
                }
            }
        }

    }
}