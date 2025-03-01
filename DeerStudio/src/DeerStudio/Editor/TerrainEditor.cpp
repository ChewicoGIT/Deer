#include "TerrainEditor.h"

#include "Deer/Core/Core.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Voxels/Voxel.h"
#include "Deer/Voxels/VoxelWorldProps.h"

#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"

#include "DeerRender/Scene/SceneCamera.h"
#include "EditorUtils.h"
#include "Viewport.h"
#include "DeerRender/Render/Texture.h"
#include "Icons.h"

#include <string>

namespace Deer {
	void deleteVoxelWorld();
	void drawVoxelRay();

	enum TerrainEditMode {
		TerrainEditMode_Add = 0,
		TerrainEditMode_Substract = 1,
		TerrainEditMode_Fill = 2,
		TerrainEditMode_Empty = 3
	};

	TerrainEditMode m_terrainEditMode = TerrainEditMode_Add;

	extern bool testing;
	void terrainEditor_onImGui() {
		ImGui::Begin("Terrain Editor");

		Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
		if (voxelWorld == nullptr) {

			ImGui::Text("Voxel world is not initialized");

			ImGui::Separator();

			ImGui::Spacing();
			static int values[3] = { 1 };
			ImGui::InputInt3("Chunk Size", values);
			
			for (int i = 0; i < 3; i++)
				if (values[i] < 1)
					values[i] = 1;
				else if (values[i] > 32)
					values[i] = 32;

			ImGui::Text("Total chunks: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[0] * values[1] * values[2]).c_str());

			ImGui::Separator();

			ImGui::Text("Units x: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[0] * 32).c_str());

			ImGui::Text("Units y: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[1] * 32).c_str());

			ImGui::Text("Units z: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[2] * 32).c_str());

			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button("Create voxel world")) {
				
				VoxelWorldProps props;
				props.chunkSizeX = values[0];
				props.chunkSizeY = values[1];
				props.chunkSizeZ = values[2];

				Project::m_scene.createVoxelWorld(props);
			}

			ImGui::End();
			return;
		}

		if (ImGui::Button("Create Fast world base")) {
			VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
			Project::m_scene.getVoxelWorld()->fillVoxels(
				0, 32 * worldProps.chunkSizeX - 1,
				0, 16 * worldProps.chunkSizeY,
				0, 32 * worldProps.chunkSizeZ - 1, 
				Voxel(1));	
		}

		if (ImGui::Button("Bake light")) {
			VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
			Project::m_scene.getVoxelWorld()->bakeAmbientLight(0, 32 * worldProps.chunkSizeX - 1, 0, 32 * worldProps.chunkSizeZ - 1);
		}

		ImGui::Separator();

		float windowWidth = ImGui::GetWindowContentRegionWidth();
		
		ImGui::Text("Editor mode:");
		if (ImGui::ImageButton((ImTextureID)add_icon->getTextureID(), ImVec2(windowWidth / 4 - 20, windowWidth / 4 - 20), ImVec2(0, 1), ImVec2(1, 0),
			-1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Add)? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Add;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)substract_icon->getTextureID(), ImVec2(windowWidth / 4 - 20, windowWidth / 4 - 20), ImVec2(0, 1), ImVec2(1, 0),
			- 1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Substract) ? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Substract;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)fill_icon->getTextureID(), ImVec2(windowWidth / 4 - 20, windowWidth / 4 - 20), ImVec2(0, 1), ImVec2(1, 0),
			-1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Fill) ? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Fill;
		}
		ImGui::SameLine();
		if (ImGui::ImageButton((ImTextureID)fill_empty_icon->getTextureID(), ImVec2(windowWidth / 4 - 20, windowWidth / 4 - 20), ImVec2(0, 1), ImVec2(1, 0),
			-1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Empty) ? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Empty;
		}

		VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
		if (testing) {
			if (ImGui::Button("notest")) {
				testing = !testing;
				Project::m_scene.getVoxelWorld()->bakeAmbientLight(0, 32 * worldProps.chunkSizeX - 1, 0, 32 * worldProps.chunkSizeZ - 1);
			}
		}
		else {
			if (ImGui::Button("test")) {
				testing = !testing;
				Project::m_scene.getVoxelWorld()->bakeAmbientLight(0, 32 * worldProps.chunkSizeX - 1, 0, 32 * worldProps.chunkSizeZ - 1);
			}
		}

		if (ImGui::Button("Delete voxel world")) {
			ImGui::OpenPopup("DELETE_VOXEL_WORLD");
		}

		deleteInputPopup<deleteVoxelWorld>("DELETE_VOXEL_WORLD", "Are you sure you want to delete voxel world?");
		drawVoxelRay();

		ImGui::End();
	}


	void deleteVoxelWorld() {
		Project::m_scene.deleteVoxelWorld();
	}

	void drawVoxelRay() {
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

		if (Project::m_scene.getVoxelWorld()) {
			VoxelRayResult res = Project::m_scene.getVoxelWorld()->rayCast_editor(viewport_sceneCamera.transform.position, rayDir);

			if (res.distance != 1000) {
				Project::m_scene.getMainGizmoRenderer().refresh();
				Project::m_scene.getMainGizmoRenderer().drawVoxelLineFace(res.xPos, res.yPos, res.zPos, res.face);

				if (viewport_isActive && ImGui::GetMouseClickedCount(0) > 0) {

					if (m_terrainEditMode == TerrainEditMode_Substract) {
						if (res.yPos >= 0) {
							Project::m_scene.getVoxelWorld()->setVoxel(res.xPos, res.yPos, res.zPos, emptyVoxel);
							Project::m_scene.getVoxelWorld()->bakeAmbientLightFromPoint(res.xPos, res.zPos);
						}
					} else if (m_terrainEditMode == TerrainEditMode_Add) {
						int xPos = res.xPos + NORMAL_DIR(0, res.face);
						int yPos = res.yPos + NORMAL_DIR(1, res.face);
						int zPos = res.zPos + NORMAL_DIR(2, res.face);
						
						Project::m_scene.getVoxelWorld()->setVoxel(xPos, yPos, zPos, Voxel(1));
						Project::m_scene.getVoxelWorld()->bakeAmbientLightFromPoint(xPos, zPos);
					}
				}
				//else if (viewport_isActive && ImGui::GetMouseClickedCount(ImGuiMouseButton_Right)) {
				//	int xPos = res.xPos;
				//	int yPos = res.yPos;
				//	int zPos = res.zPos;
				//
				//	Project::m_scene.getVoxelWorld()->setVoxel(xPos, yPos, zPos, Voxel(0));
				//	Project::m_scene.getVoxelWorld()->bakeAmbientLightFromPoint(xPos, zPos);
				//}
			}
		}
	}

}