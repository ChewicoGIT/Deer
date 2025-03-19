#include "TerrainEditor.h"

#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Voxels/VoxelWorld.h"

#include "imgui.h"
#include <string>

namespace Deer{
    void TerrainEditor::createVoxelWorldPopup() {
        if (!ImGui::BeginPopup(TERRAIN_EDITOR_CREATE_VOXEL_WORLD_POPUP_NAME))
            return;

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
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}