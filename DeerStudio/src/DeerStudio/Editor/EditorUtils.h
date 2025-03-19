#pragma once
#include "imgui.h"

namespace Deer {

	template <void (*action_function)(const std::string&)>
	void stringInputPopup(const char* nameID, const char* inputInfo) {

        static char name_buffer[256];
        name_buffer[0] = '\0';

        if (ImGui::BeginPopup(nameID)) {
            ImGui::Text("%s", inputInfo);
            
            ImGui::InputText("##nameInputPopup", name_buffer, 256, ImGuiInputTextFlags_EnterReturnsTrue);

            bool pressed_enter = ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter));
            if (pressed_enter) {

                std::string name(name_buffer);
                action_function(name);

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
	}

    template <void (*action_function)(bool save)>
    void saveInputPopup(const char* nameID, const char* inputInfo) {

        if (ImGui::BeginPopup(nameID)) {
            ImGui::Text("%s", inputInfo);

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(100, 255, 120, 255));
            bool save = ImGui::Button("Save");
            ImGui::PopStyleColor();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 100, 120, 255));
            ImGui::SameLine();
            bool dont_save = ImGui::Button("Don't save");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            bool cancel = ImGui::Button("Cancel");

            if (save) {
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                action_function(true);
                return;
            } else if (dont_save) {
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                action_function(false);
                return;
            } else if (cancel) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    template <void (*action_function)(void)>
    void deleteInputPopup(const char* nameID, const char* inputInfo) {

        if (ImGui::BeginPopup(nameID)) {
            ImGui::Text("%s", inputInfo);

            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 122, 122, 255));
            bool delete_button = ImGui::Button("Delete");
            ImGui::PopStyleColor();
            ImGui::SameLine();
            bool cancel_button = ImGui::Button("Cancel");

            if (delete_button) {
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
                action_function();
                return;
            }
            else if (cancel_button) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

    }

    inline void setupColumns(int iconSize) {
        float width = ImGui::GetWindowContentRegionWidth();

        if (width < iconSize) {
            ImGui::Columns();
            return;
        }

        int cols = (int)(width / (iconSize));
        float componentWidth = width / (float)cols;

        ImGui::Columns(cols, 0, false);
    }


	inline bool iconButton(ImTextureID textureID, int size, bool active) {
		return ImGui::ImageButton(textureID, 
			ImVec2(size, size),
			ImVec2(0, 1),
			ImVec2(1, 0), -1,
			ImVec4(0, 0, 0, 0),
			ImVec4(active? 1 : 0.8f, active? 1 : 0.8f, active? 1 : 0.8f, 1));
	}


	inline bool iconButton(ImTextureID textureID, int size, bool active, ImVec2 min, ImVec2 max) {
		return ImGui::ImageButton(textureID, 
			ImVec2(size, size),
			min,
			max, -1,
			ImVec4(0, 0, 0, 0),
			ImVec4(active? 1 : 0.8f, active? 1 : 0.8f, active? 1 : 0.8f, 1));
	}
}