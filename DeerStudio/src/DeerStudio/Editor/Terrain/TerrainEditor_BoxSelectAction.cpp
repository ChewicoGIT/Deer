#include "TerrainEditor.h"

#include "DeerStudio/Project.h"
#include "Deer/Core/Log.h"

#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"
#include "Deer/Scene.h"

#include "DeerRender/GizmoRenderer.h"
#include "DeerRender/Render/Texture.h"

#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Editor/EditorUtils.h"
#include "DeerStudio/Editor/Icons.h"

#include "imgui.h"
#define FACE_VOXEL_SELECT 0
#define INTERNAL_VOXEL_SELECT 1

namespace Deer {
    namespace TerrainEditor {
        VoxelCordinates selectedVoxel_cache(-1, -1, -1);
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

        if (!viewportIsActive()) {
            selectedVoxel_cache.makeNull();
            return;
        }

        Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
        GizmoRenderer& gizmo = Project::m_scene.getMainGizmoRenderer();

        VoxelCordinates currentSelectedVoxelCoords;
        if (voxelSelectMode == FACE_VOXEL_SELECT) {
            currentSelectedVoxelCoords = voxelFaceRayCoords;
        } else {
            currentSelectedVoxelCoords = voxelRayCoords;
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) == 1) {
            selectedVoxel_cache = currentSelectedVoxelCoords;
        }

        if (!selectedVoxel_cache.isNull() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            gizmo.drawVoxelLine(selectedVoxel_cache.x, selectedVoxel_cache.y, selectedVoxel_cache.z, glm::vec3(1, 0.7f, 0.7f));
        }

        if (!selectedVoxel_cache.isNull() && !currentSelectedVoxelCoords.isNull() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            VoxelCordinates min;
            VoxelCordinates max;

            if (currentSelectedVoxelCoords.x < selectedVoxel_cache.x) {
                min.x = currentSelectedVoxelCoords.x;
                max.x = selectedVoxel_cache.x;
            } else {
                max.x = currentSelectedVoxelCoords.x;
                min.x = selectedVoxel_cache.x;
            }

            if (currentSelectedVoxelCoords.y < selectedVoxel_cache.y) {
                min.y = currentSelectedVoxelCoords.y;
                max.y = selectedVoxel_cache.y;
            } else {
                max.y = currentSelectedVoxelCoords.y;
                min.y = selectedVoxel_cache.y;
            }

            if (currentSelectedVoxelCoords.z < selectedVoxel_cache.z) {
                min.z = currentSelectedVoxelCoords.z;
                max.z = selectedVoxel_cache.z;
            } else {
                max.z = currentSelectedVoxelCoords.z;
                min.z = selectedVoxel_cache.z;
            }

            // Z Face
            for (int x = min.x; x <= max.x; x++) {
                for (int y = min.y; y <= max.y; y++) {
                    gizmo.drawVoxelLineFace(x, y, min.z, NORMAL_BACK, glm::vec3(1, 0.8f, 0.8f));
                    gizmo.drawVoxelLineFace(x, y, max.z, NORMAL_FRONT, glm::vec3(1, 0.8f, 0.8f));
                }
            }

            // Y Face
            for (int x = min.x; x <= max.x; x++) {
                for (int z = min.z; z <= max.z; z++) {
                    gizmo.drawVoxelLineFace(x, min.y, z, NORMAL_DOWN, glm::vec3(1, 0.8f, 0.8f));
                    gizmo.drawVoxelLineFace(x, max.y, z, NORMAL_UP, glm::vec3(1, 0.8f, 0.8f));
                }
            }

            // X Face
            for (int y = min.y; y <= max.y; y++) {
                for (int z = min.z; z <= max.z; z++) {
                    gizmo.drawVoxelLineFace(min.x, y, z, NORMAL_LEFT, glm::vec3(1, 0.8f, 0.8f));
                    gizmo.drawVoxelLineFace(max.x, y, z, NORMAL_RIGHT, glm::vec3(1, 0.8f, 0.8f));
                }
            }
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            
        }
    }
}