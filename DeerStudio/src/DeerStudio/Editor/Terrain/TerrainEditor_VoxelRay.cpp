#include "Deer/Scene.h"
#include "Deer/Voxel.h"
#include "Deer/VoxelWorld.h"
#include "DeerRender/SceneCamera.h"
#include "DeerStudio/Editor/Viewport.h"
#include "DeerStudio/Project.h"
#include "TerrainEditor.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"

namespace Deer {
	namespace TerrainEditor {
		VoxelCordinates voxelRayCoords;
		VoxelCordinates voxelFaceRayCoords;
	}  // namespace TerrainEditor

	void TerrainEditor::voxelRay() {
		if (viewport_relativeXMouse < 0 || viewport_relativeXMouse > 1 ||
		    viewport_relativeYMouse < 0 || viewport_relativeYMouse > 1)
			return;

		glm::mat4 camMatrix =
		    glm::inverse(viewport_sceneCamera.transform.getMatrix());
		glm::mat4 projectionMatrix = viewport_sceneCamera.camera.getMatrix();
		glm::mat4 invertZ = glm::scale(glm::mat4(1.0f), glm::vec3(1, 1, -1));

		glm::mat4 cameraProjectionMatrix =
		    projectionMatrix * invertZ * camMatrix;
		cameraProjectionMatrix = glm::inverse(cameraProjectionMatrix);

		glm::vec4 nearPoint = cameraProjectionMatrix *
		                      glm::vec4(viewport_relativeXMouse * 2 - 1,
		                                viewport_relativeYMouse * 2 - 1, -1, 1);
		glm::vec4 farPoint = cameraProjectionMatrix *
		                     glm::vec4(viewport_relativeXMouse * 2 - 1,
		                               viewport_relativeYMouse * 2 - 1, 1, 1);

		nearPoint /= nearPoint.w;
		farPoint /= farPoint.w;

		glm::vec3 rayDir = farPoint - nearPoint;
		rayDir = glm::normalize(rayDir);

		VoxelRayResult res = Project::m_scene.getVoxelWorld()->rayCast_editor(
		    viewport_sceneCamera.transform.position, rayDir, 50);

		voxelRayCoords.makeNull();
		voxelFaceRayCoords.makeNull();
		if (res.distance != 50) {
			voxelRayCoords = res.hitPos;
			voxelFaceRayCoords =
			    VoxelCordinates(res.hitPos.x + NORMAL_DIR(0, res.face),
			                    res.hitPos.y + NORMAL_DIR(1, res.face),
			                    res.hitPos.z + NORMAL_DIR(2, res.face));
			Project::m_scene.getMainGizmoRenderer().drawVoxelLineFace(
			    res.hitPos.x, res.hitPos.y, res.hitPos.z, res.face);

			if (viewport_isActive && ImGui::GetMouseClickedCount(0) > 0) {
				if (terrainEditMode == TerrainEditMode_Substract) {
					if (res.hitPos.y >= 0) {
						Project::m_scene.getVoxelWorld()->setVoxel(res.hitPos,
						                                           emptyVoxel);
					}
				} else if (terrainEditMode == TerrainEditMode_Add) {
					VoxelCordinates position(
					    res.hitPos.x + NORMAL_DIR(0, res.face),
					    res.hitPos.y + NORMAL_DIR(1, res.face),
					    res.hitPos.z + NORMAL_DIR(2, res.face));

					Project::m_scene.getVoxelWorld()->setVoxel(
					    position, Voxel(selectedVoxelID));
				}
			}
		}
	}
}  // namespace Deer