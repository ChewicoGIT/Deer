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
#define FACE_VOXEL_SELECT 0
#define INTERNAL_VOXEL_SELECT 1

namespace Deer {
    namespace TerrainEditor {
        VoxelCordinates selectedVoxelStart_cache(-1, -1, -1);
        VoxelCordinates selectedVoxelEnd_cache(-1, -1, -1);
        uint8_t voxelSelectMode = 0;
    }

    void TerrainEditor::boxSelect() {
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Select mode: ");
        ImGui::SameLine();
        if (voxelSelectMode == FACE_VOXEL_SELECT)
            ImGui::Text("Face");
        else
            ImGui::Text("Voxel");
        
        setupColumns(ICON_BTN_MIN_SIZE + 16);
        if (iconButton((ImTextureID)(uint64_t)Icons::face_voxel_selection->getTextureID(), ICON_BTN_MIN_SIZE, voxelSelectMode == FACE_VOXEL_SELECT)) {
            voxelSelectMode = FACE_VOXEL_SELECT;
        }
        ImGui::Text("Face");
        ImGui::NextColumn();
        if (iconButton((ImTextureID)(uint64_t)Icons::internal_voxel_selection->getTextureID(), ICON_BTN_MIN_SIZE, voxelSelectMode == INTERNAL_VOXEL_SELECT)) {
            voxelSelectMode = INTERNAL_VOXEL_SELECT;
        }
        ImGui::Text("Voxel");
        ImGui::Columns();

        Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
        GizmoRenderer& gizmo = Project::m_scene.getMainGizmoRenderer();


        if (!selectedVoxelStart_cache.isNull() && !selectedVoxelEnd_cache.isNull()) {
            VoxelCordinates min;
            VoxelCordinates max;

            if (selectedVoxelEnd_cache.x < selectedVoxelStart_cache.x) {
                min.x = selectedVoxelEnd_cache.x;
                max.x = selectedVoxelStart_cache.x;
            } else {
                max.x = selectedVoxelEnd_cache.x;
                min.x = selectedVoxelStart_cache.x;
            }

            if (selectedVoxelEnd_cache.y < selectedVoxelStart_cache.y) {
                min.y = selectedVoxelEnd_cache.y;
                max.y = selectedVoxelStart_cache.y;
            } else {
                max.y = selectedVoxelEnd_cache.y;
                min.y = selectedVoxelStart_cache.y;
            }

            if (selectedVoxelEnd_cache.z < selectedVoxelStart_cache.z) {
                min.z = selectedVoxelEnd_cache.z;
                max.z = selectedVoxelStart_cache.z;
            } else {
                max.z = selectedVoxelEnd_cache.z;
                min.z = selectedVoxelStart_cache.z;
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

        if (!viewportIsActive()) {
            if (selectedVoxelStart_cache.isNull())
                selectedVoxelStart_cache.makeNull();
            return;
        }

        if (ImGui::IsMouseDragging(ImGuiMouseButton_Left) || ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            
            if (voxelSelectMode == FACE_VOXEL_SELECT) {
                selectedVoxelEnd_cache = voxelFaceRayCoords;
            } else {
                selectedVoxelEnd_cache = voxelRayCoords;
            }
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) == 1) {
            selectedVoxelStart_cache = selectedVoxelEnd_cache;
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle) == 1) {
            selectedVoxelStart_cache.makeNull();
            selectedVoxelEnd_cache.makeNull();
        }

        if (!selectedVoxelStart_cache.isNull() && !selectedVoxelEnd_cache.isNull() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            gizmo.drawVoxelLine(selectedVoxelStart_cache.x, selectedVoxelStart_cache.y, selectedVoxelStart_cache.z, glm::vec3(1, 0.7f, 0.7f));
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            
        }
    }
}