#include "TerrainEditor.h"

#include "Deer/Core/Core.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Voxels/VoxelWorld.h"
#include "Deer/Voxels/VoxelWorldProps.h"

#include "imgui.h"

#include <string>

namespace Deer {
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

			ImGui::Spacing();

			if (ImGui::Button("Create voxel world")) {
				
				VoxelWorldProps props;
				props.chunkSizeX = values[0];
				props.chunkSizeY = values[1];
				props.chunkSizeZ = values[2];

				Project::m_scene.createVoxelWorld(props);
			}

			return ImGui::End();
		}

		ImGui::End();
	}
}