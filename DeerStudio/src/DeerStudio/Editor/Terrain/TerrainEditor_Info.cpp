#include "TerrainEditor.h"
#include "DeerStudio/Project.h"

#include "DeerStudio/Editor/Icons.h"
#include "DeerStudio/Editor/EditorUtils.h"

#include "Deer/Voxel.h"
#include "Deer/Scene.h"
#include "Deer/VoxelWorld.h"

#include "imgui.h"
#include <string>

namespace Deer {
    namespace TerrainEditor {
		void deleteVoxelWorld();
	}

    void TerrainEditor::info() {
		Ref<VoxelWorld>& voxelWorld = Project::m_scene.getVoxelWorld();
        VoxelWorldProps worldProps = voxelWorld->getVoxelWorldProps();

        ImGui::Text("Voxel world chunk and voxel size:");
        ImGui::Text(" x: %s chunks -> %s voxels", std::to_string(worldProps.chunkSizeX).c_str(), std::to_string(worldProps.chunkSizeX * CHUNK_SIZE_X).c_str());
        ImGui::Text(" y: %s chunks -> %s voxels", std::to_string(worldProps.chunkSizeY).c_str(), std::to_string(worldProps.chunkSizeY * CHUNK_SIZE_Y).c_str());
        ImGui::Text(" z: %s chunks -> %s voxels", std::to_string(worldProps.chunkSizeZ).c_str(), std::to_string(worldProps.chunkSizeZ * CHUNK_SIZE_Z).c_str()); 

        ImGui::Separator();


        int64_t voxelCount = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * worldProps.chunkSizeX * worldProps.chunkSizeY * worldProps.chunkSizeZ;
        int64_t clientSize = voxelCount * (sizeof(Voxel) + sizeof(VoxelLight));
        int64_t serverSize = voxelCount * (sizeof(Voxel));

        ImGui::Text("%s", "Theorical max voxel count : ");
        ImGui::SameLine();
        ImGui::Text("%s", std::to_string(voxelCount).c_str());

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("%s", "Client theorical max size : ");
        ImGui::Text(" %sB", std::to_string(clientSize).c_str());
        ImGui::Text(" +%sKB", std::to_string(clientSize / 1024).c_str());
        ImGui::Text(" +%sMB", std::to_string(clientSize / (1024 * 1024)).c_str());
        ImGui::Text(" +%sGB", std::to_string(clientSize / (1024 * 1024 * 1024)).c_str());

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("%s", "Server theorical max size : ");
        ImGui::Text(" %sB", std::to_string(serverSize).c_str());
        ImGui::Text(" +%sKB", std::to_string(serverSize / 1024).c_str());
        ImGui::Text(" +%sMB", std::to_string(serverSize / (1024 * 1024)).c_str());
        ImGui::Text(" +%sGB", std::to_string(serverSize / (1024 * 1024 * 1024)).c_str());


        ImGui::Separator();
        ImGui::Spacing();


		if (ImGui::Button("Delete voxel world")) {
			ImGui::OpenPopup("DELETE_VOXEL_WORLD");
		}

		// TEMP
		if (ImGui::Button("Create Ceiling")) {
			Project::m_scene.getVoxelWorld()->fillVoxels(
				0, 32 * worldProps.chunkSizeX - 1,
				0, 16,
				0, 32 * worldProps.chunkSizeZ - 1, 
				Voxel(VoxelData::getVoxelID("wood")));
			
			Project::m_scene.getVoxelWorld()->fillVoxels(
				1, 32 * worldProps.chunkSizeX - 2,
				8, 15,
				1, 32 * worldProps.chunkSizeZ - 2, 
				Voxel(VoxelData::getVoxelID("air")));
		}

		deleteInputPopup<deleteVoxelWorld>("DELETE_VOXEL_WORLD", "Are you sure you want to delete voxel world?");
    }

	void TerrainEditor::deleteVoxelWorld() {
		Project::m_scene.deleteVoxelWorld();
	}
}