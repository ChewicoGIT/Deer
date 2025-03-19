#include "TerrainEditor.h"

#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"

#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/VoxelWorldProps.h"

#include "DeerRender/Scene/SceneCamera.h"

#include "DeerStudio/Editor/Viewport.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

namespace Deer {
    namespace TerrainEditor {
        VoxelCordinates voxelRayCoords;
        VoxelCordinates voxelFaceRayCoords;
    }

	void TerrainEditor::voxelRay() {
		if (viewport_relativeXMouse < 0 || viewport_relativeXMouse > 1 || viewport_relativeYMouse < 0 || viewport_relativeYMouse > 1)
			return;

		glm::mat4 camMatrix = glm::inverse(viewport_sceneCamera.transform.getMatrix());
		glm::mat4 projectionMatrix = viewport_sceneCamera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		glm::mat4 cameraProjectionMatrix = projectionMatrix * invertZ * camMatrix;
		cameraProjectionMatrix = glm::inverse(cameraProjectionMatrix);

		glm::vec4 nearPoint = cameraProjectionMatrix * glm::vec4(viewport_relativeXMouse * 2 - 1, viewport_relativeYMouse * 2 - 1, -1, 1);
		glm::vec4 farPoint = cameraProjectionMatrix * glm::vec4(viewport_relativeXMouse * 2 - 1, viewport_relativeYMouse * 2 - 1, 1, 1);

		nearPoint /= nearPoint.w;
		farPoint /= farPoint.w;

		glm::vec3 rayDir = farPoint - nearPoint;
		rayDir = glm::normalize(rayDir);

        VoxelRayResult res = Project::m_scene.getVoxelWorld()->rayCast_editor(viewport_sceneCamera.transform.position, rayDir, 50);

        voxelRayCoords.makeNull();
        voxelFaceRayCoords.makeNull();
        if (res.distance != 50) {
            voxelRayCoords = VoxelCordinates(res.xPos, res.yPos, res.zPos);
            voxelFaceRayCoords = VoxelCordinates(
                res.xPos +  NORMAL_DIR(0, res.face),
                res.yPos +  NORMAL_DIR(1, res.face),
                res.zPos +  NORMAL_DIR(2, res.face)
            );
            Project::m_scene.getMainGizmoRenderer().drawVoxelLineFace(res.xPos, res.yPos, res.zPos, res.face);

            if (viewport_isActive && ImGui::GetMouseClickedCount(0) > 0) {
                if (terrainEditMode == TerrainEditMode_Substract) {
                    if (res.yPos >= 0) {
                        Project::m_scene.getVoxelWorld()->setVoxel(res.xPos, res.yPos, res.zPos, emptyVoxel);
                        Project::m_scene.getVoxelWorld()->bakeAmbientLightFromPoint(res.xPos, res.zPos);
                        Project::m_scene.getVoxelWorld()->bakeVoxelLightFromPoint(res.xPos, res.yPos, res.zPos);
                    }
                } else if (terrainEditMode == TerrainEditMode_Add) {
                    int xPos = res.xPos + NORMAL_DIR(0, res.face);
                    int yPos = res.yPos + NORMAL_DIR(1, res.face);
                    int zPos = res.zPos + NORMAL_DIR(2, res.face);
                    
                    Project::m_scene.getVoxelWorld()->setVoxel(xPos, yPos, zPos, Voxel(selectedVoxelID));
                    Project::m_scene.getVoxelWorld()->bakeAmbientLightFromPoint(xPos, zPos);
                    Project::m_scene.getVoxelWorld()->bakeVoxelLightFromPoint(res.xPos, res.yPos, res.zPos);
                }
            }
        }
	}
}