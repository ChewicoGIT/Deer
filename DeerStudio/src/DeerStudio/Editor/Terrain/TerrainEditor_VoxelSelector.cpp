#include "TerrainEditor.h"

#include "Deer/Voxel.h"
#include "Deer/VoxelData.h"

#include "DeerRender/Voxels/VoxelAspect.h"
#include "DeerRender/Render/Texture.h"

#include "DeerStudio/Editor/EditorUtils.h"
#include "DeerStudio/Editor/Icons.h"

#include "imgui.h"

namespace Deer {
    namespace TerrainEditor {
        char filterChar[256];
    }

    void TerrainEditor::voxelSelector() {
        int textureSize = VoxelData::getVoxelTextureAtlasSize();
        VoxelAspect& selectedVoxelAspect = VoxelData::voxelsAspect[selectedVoxelID];

        int selectedVoxel_posY = selectedVoxelAspect.textureFacesIDs[NORMAL_BACK] / textureSize;
        int selectedVoxel_posX = selectedVoxelAspect.textureFacesIDs[NORMAL_BACK] - selectedVoxel_posY * textureSize;

        float selectedVoxel_minX = (float)(selectedVoxel_posX + 0) / (float)textureSize;
        float selectedVoxel_minY = (float)(selectedVoxel_posY + 0) / (float)textureSize;
        float selectedVoxel_maxX = (float)(selectedVoxel_posX + 1) / (float)textureSize;
        float selectedVoxel_maxY = (float)(selectedVoxel_posY + 1) / (float)textureSize;

        ImGui::Text("Selected Voxel: %s", selectedVoxelAspect.definition.voxelName.c_str());
        iconButton(
            (ImTextureID)(uint64_t)VoxelData::getVoxelColorTextureAtlas()->getTextureID(),
            ICON_BTN_MIN_SIZE,
            true,
            ImVec2(selectedVoxel_minX, selectedVoxel_maxY),
            ImVec2(selectedVoxel_maxX, selectedVoxel_minY));
        
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Filter: ");
        ImGui::SameLine();
        ImGui::InputText("##Filter", filterChar, 255);
        if (filterChar[0] != 0) {
            if (ImGui::Button("Clear filter")) {
                filterChar[0] = 0;
            }
        }

        ImGui::BeginChild("VOXEL_SELECTOR_CHILD", ImVec2(0, 124), ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_AlwaysVerticalScrollbar) ;

        setupColumns(ICON_BTN_MIN_SIZE + 10);

        for (int id = 0; id < VoxelData::voxelsInfo.size(); id ++) {
            VoxelInfo& vi = VoxelData::voxelsInfo[id];
            if (vi.type != VoxelInfoType::Voxel)
                continue;

            if (filterChar[0] != 0) {
                if (vi.name.find(filterChar) == std::string::npos) 
                continue;
            }

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
            if (iconButton(
                (ImTextureID)(uint64_t)VoxelData::getVoxelColorTextureAtlas()->getTextureID(),
                ICON_BTN_MIN_SIZE,
                selectedVoxelID == id,
                ImVec2(minX, maxY),
                ImVec2(maxX, minY))) {
                    selectedVoxelID = id;
            }
            ImGui::Text("%s", va.definition.voxelName.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Columns();
        ImGui::EndChild();
        ImGui::Separator();
    }
}