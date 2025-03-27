#include "TerrainEditor.h"
#include "DeerStudio/Project.h"
#include "DeerStudio/Editor/Icons.h"
#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Editor/EditorUtils.h"

#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"
#include "Deer/Scene.h"

#include "DeerRender/GizmoRenderer.h"

#include "imgui.h"

namespace Deer {
    void TerrainEditor::empty() {
        if (!viewportIsActive())
            return;

        VoxelCordinates selectVoxel;
        VoxelCordinates clampedCordinates;
        selectVoxel = voxelRayCoords;

        clampedCordinates = selectVoxel;
        Project::m_scene.getVoxelWorld()->getVoxelWorldProps().clampCordinates(clampedCordinates);

        if (clampedCordinates != selectVoxel)
            return;
        
        VoxelCordinates min = selectedVoxelStart;
        VoxelCordinates max = selectedVoxelEnd;

        if (min.isNull() || max.isNull()) {
            min = VoxelCordinates(selectVoxel.x - 16, selectVoxel.y - 16, selectVoxel.z - 16);
            max = VoxelCordinates(selectVoxel.x + 15, selectVoxel.y + 15, selectVoxel.z + 15);
        }
        Project::m_scene.getVoxelWorld()->getVoxelWorldProps().clampAndSetMinMax(min, max);

        if (selectVoxel.x < min.x || selectVoxel.x > max.x ||
            selectVoxel.y < min.y || selectVoxel.y > max.y ||
            selectVoxel.z < min.z || selectVoxel.z > max.z)
            return;

        for (int i = 0; i < 6; i++) {
            Project::m_scene.getMainGizmoRenderer().drawVoxelFace(clampedCordinates.x, clampedCordinates.y, clampedCordinates.z,
                VoxelData::getVoxelID("debug"), i, 0);
        }

        if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left)) {
            Voxel voxel = Project::m_scene.getVoxelWorld()->readVoxel(selectVoxel);
            Project::m_scene.getVoxelWorld()->remplaceVoxels(min, max, voxel, Voxel(0));
        }
    }
}