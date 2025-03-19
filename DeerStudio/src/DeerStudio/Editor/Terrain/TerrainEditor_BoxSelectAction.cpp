#include "TerrainEditor.h"

#include "Deer/Core/Project.h"
#include "Deer/Core/Log.h"

#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Scene/Scene.h"

#include "DeerRender/Scene/GizmoRenderer.h"

#include "DeerStudio/Editor/Viewport.h"

#include "imgui.h"

namespace Deer {
    namespace TerrainEditor {
        VoxelCordinates selectedVoxel_cache(-1, -1, -1);
    }

    void TerrainEditor::boxSelect() {
        if (!viewport_isActive) {
            selectedVoxel_cache.makeNull();
            return;
        }

        Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
        GizmoRenderer& gizmo = Project::m_scene.getMainGizmoRenderer();

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) == 1) {
            selectedVoxel_cache = voxelFaceRayCoords;
        }

        if (!selectedVoxel_cache.isNull() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            gizmo.drawVoxelLine(selectedVoxel_cache.x, selectedVoxel_cache.y, selectedVoxel_cache.z, glm::vec3(1, 0.7f, 0.7f));
        }


        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            
        }
    }
}