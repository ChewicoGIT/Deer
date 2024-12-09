#include "VoxelPannel.h"
#include "imgui.h"

namespace Deer {
	void VoxelPannel::onImGui() {
        ImGui::Begin("Voxel");

        if (ImGui::BeginTable("Block List", 3)) {
            ImGui::TableSetupColumn("ID");
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Actions");
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();

            // Block ID
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", 20);

            // Block Name
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", "block.name.c_str()");

            // Actions
            ImGui::TableSetColumnIndex(2);
            if (ImGui::Button("Edit##20")) {
                m_selectedVoxel = 20;  // Set selected block for editing
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete##20")) {
                // Handle deletion logic
            }

            ImGui::EndTable();
        }

        // Add Block Button
        if (ImGui::Button("Add New Block")) {
            
        }
		ImGui::End();

        if (m_selectedVoxel != 0) {
            if (ImGui::Begin("Block Properties")) {
                // Block Name
                char nameBuffer[128];
                if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer) - 1)) {
                    
                }

                // Block Texture
                ImGui::Text("Texture:");
                ImGui::SameLine();
                if (ImGui::Button("block.texture.c_str()")) {
                    // Open texture selection popup
                }

                // Block ID
                int id;
                ImGui::InputInt("ID", &id);

                // Transparency
                bool transparent = true;
                if (ImGui::Checkbox("Transparent", &transparent)) {
                    //block.isTransparent = transparent;
                }

                // Light Emission
                //ImGui::SliderInt("Light Emission", &block.lightEmission, 0, 15);

                ImGui::End();
            }
        }
	}
}