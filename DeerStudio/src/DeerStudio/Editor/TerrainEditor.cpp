#include "TerrainEditor.h"

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

	uint16_t selectedVoxelID = 1;
	TerrainEditMode m_terrainEditMode = TerrainEditMode_Add;

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
			ImGui::Text("%s", std::to_string(values[0] * values[1] * values[2]).c_str());

			ImGui::Separator();

			ImGui::Text("%s", "Units x: ");
			ImGui::SameLine();
			ImGui::Text("%s", std::to_string(values[0] * 32).c_str());

			ImGui::Text("%s", "Units y: ");
			ImGui::SameLine();
			ImGui::Text("%s", std::to_string(values[1] * 32).c_str());

			ImGui::Text("%s", "Units z: ");
			ImGui::SameLine();
			ImGui::Text("%s", std::to_string(values[2] * 32).c_str());

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
				0, 10,
				0, 32 * worldProps.chunkSizeZ - 1, 
				Voxel(VoxelData::getVoxelID("dirt")));	
		}

		if (ImGui::Button("Create Ceiling")) {
			VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
			Project::m_scene.getVoxelWorld()->fillVoxels(
				0, 32 * worldProps.chunkSizeX - 1,
				16, 16,
				0, 32 * worldProps.chunkSizeZ - 1, 
				Voxel(VoxelData::getVoxelID("wood")));	
		}

		if (ImGui::Button("Bake light")) {
			VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
			Project::m_scene.getVoxelWorld()->bakeAmbientLight(0, 32 * worldProps.chunkSizeX - 1, 0, 32 * worldProps.chunkSizeZ - 1);
			Project::m_scene.getVoxelWorld()->bakeVoxelLight(0, 32 * worldProps.chunkSizeX - 1, 0, 32 * worldProps.chunkSizeY - 1, 0, 32 * worldProps.chunkSizeZ - 1);
		}

		ImGui::Separator();
		ImGui::Text("Editor mode:");

		setupColumns(ICON_BTN_MIN_SIZE + 10);

		if (ImGui::ImageButton((ImTextureID)(uint64_t)add_icon->getTextureID(), ImVec2(ICON_BTN_MIN_SIZE, ICON_BTN_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0),
			-1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Add)? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Add;
		}
		ImGui::NextColumn();
		if (ImGui::ImageButton((ImTextureID)(uint64_t)substract_icon->getTextureID(), ImVec2(ICON_BTN_MIN_SIZE, ICON_BTN_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0),
			- 1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Substract) ? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Substract;
		}
		ImGui::NextColumn();
		if (ImGui::ImageButton((ImTextureID)(uint64_t)fill_icon->getTextureID(), ImVec2(ICON_BTN_MIN_SIZE, ICON_BTN_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0),
			-1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Fill) ? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Fill;
		}
		ImGui::NextColumn();
		if (ImGui::ImageButton((ImTextureID)(uint64_t)fill_empty_icon->getTextureID(), ImVec2(ICON_BTN_MIN_SIZE, ICON_BTN_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0),
			-1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, (m_terrainEditMode == TerrainEditMode_Empty) ? 0.7f : 1.0f, 1.0f))) {
			m_terrainEditMode = TerrainEditMode_Empty;
		}

		ImGui::Columns();

		if (m_terrainEditMode == TerrainEditMode_Add) {
			ImGui::Text("Voxel: ");

			setupColumns(ICON_BTN_MIN_SIZE + 10);

			int textureSize = VoxelData::getVoxelTextureAtlasSize();
			for (int id = 0; id < VoxelData::voxelsInfo.size(); id ++) {
				VoxelInfo& vi = VoxelData::voxelsInfo[id];
				if (vi.type != VoxelInfoType::Voxel)
					continue;

				ImGui::PushID(id);
				VoxelAspect& va = VoxelData::voxelsAspect[id];
				int textureID = va.textureFacesIDs[NORMAL_FRONT];

				int posY = textureID / textureSize;
				int posX = textureID - posY * textureSize;

				float minX = (float)(posX + 0) / (float)textureSize;
				float minY = (float)(posY + 0) / (float)textureSize;
				float maxX = (float)(posX + 1) / (float)textureSize;
				float maxY = (float)(posY + 1) / (float)textureSize;

				float wColor = (id == selectedVoxelID)? 1 : 0.6f;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)VoxelData::getVoxelColorTextureAtlas()->getTextureID(),
					ImVec2(ICON_BTN_MIN_SIZE, ICON_BTN_MIN_SIZE),
					ImVec2(minX, maxY), ImVec2(maxX, minY),
					-1,
					ImVec4(0, 0, 0, 0), ImVec4(wColor, wColor, wColor, 1.0f))) {
						selectedVoxelID = id;
				}
				ImGui::Text("%s", va.definition.voxelName.c_str());

				ImGui::NextColumn();
				ImGui::PopID();
			}

			ImGui::Columns();
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

			Project::m_scene.getMainGizmoRenderer().refresh();
			if (res.distance != 1000) {
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
						
						Project::m_scene.getVoxelWorld()->setVoxel(xPos, yPos, zPos, Voxel(selectedVoxelID));
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