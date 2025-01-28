#include "TerrainEditor.h"

#include "Deer/Core/Core.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Voxels/VoxelWorld.h"

#include "imgui.h"

#include <string>

namespace Deer {
	void terrainEditor_onImGui() {
		ImGui::Begin("Terrain Editor");

		Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
		if (voxelWorld == nullptr) {

			ImGui::Text("Voxel world is not initialized");

			ImGui::Spacing();
			static int values[3] = { 1 };
			ImGui::InputInt3("Chunk Size", values);
			
			for (int i = 0; i < 3; i++)
				if (values[i] < 1)
					values[i] = 1;
				else if (values[i] > 32)
					values[i] = 32;

			ImGui::Text("voxels x: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[0] * 32).c_str());
			ImGui::SameLine();
			ImGui::Text(", y: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[1] * 32).c_str());
			ImGui::SameLine();
			ImGui::Text(", z: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[2] * 32).c_str());

			ImGui::Text("Total voxels: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[0] * values[1] * values[2] * 32 * 32 * 32).c_str());

			ImGui::Text("Theorical max size: ");
			ImGui::SameLine();
			ImGui::Text(std::to_string(values[0] * values[1] * values[2] * 32 * 32 * 32).c_str());

			ImGui::Spacing();

			if (ImGui::Button("Initialize voxel world")) {

			}

			return ImGui::End();
		}

		ImGui::End();
	}
}