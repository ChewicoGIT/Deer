#include "TerrainEditor.h"

#include "Deer/Core/Core.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Voxels/VoxelWorldProps.h"

#include "imgui.h"

#include "EditorUtils.h"

#include <string>

namespace Deer {
	void deleteVoxelWorld();

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

		if (ImGui::Button("Create Base")) {

			VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();
			for (int x = 0; x < 0 * worldProps.chunkSizeX; x++) {
				for (int y = 0; y < 1; y++) {
					for (int z = 0; z < 32 * worldProps.chunkSizeZ; z++) {
						Project::m_scene.getVoxelWorld()->modVoxel(x, y, z).id = 1;
					}
				}
			}

		}

		ImGui::Separator();

        // Main Content
        ImGui::BeginChild("LeftPanel", ImVec2(200, 0), true);
        {
            // Brush Tools
			int ci = 0;
			int s = 0;
			float st = 0;
            ImGui::Text("Brush Tools");
            ImGui::Combo("Brush Type", &ci, "Cube\0Sphere\0Flatten\0Smooth\0");
            ImGui::SliderInt("Brush Size", &s, 1, 32);
            ImGui::SliderFloat("Strength", &st, 0.0f, 1.0f);

            // Material Palette
            ImGui::Separator();
            ImGui::Text("Materials");
            ImGui::ColorButton("Grass", ImVec4(0, 1, 0, 1));
            ImGui::SameLine();
            ImGui::ColorButton("Dirt", ImVec4(0.5f, 0.3f, 0.1f, 1));
            // Add more materials...
        }
        ImGui::EndChild();

		if (ImGui::Button("Delete voxel world")) {
			ImGui::OpenPopup("DELETE_VOXEL_WORLD");
		}

		deleteInputPopup<deleteVoxelWorld>("DELETE_VOXEL_WORLD", "Are you sure you want to delete voxel world?");

		ImGui::End();
	}


	void deleteVoxelWorld() {
		Project::m_scene.deleteVoxelWorld();
	}
}