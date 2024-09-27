#include "AssetManagerPannel.h"
#include "Deer/Core/Log.h"
#include "Deer/Render/Texture.h"
#include "imgui.h"

namespace Deer {

    namespace fs = std::filesystem;

    AssetManagerPannel::AssetManagerPannel() 
        : m_currentPath("assets") {

        m_folderIcon = Texture2D::create("editor/icons/folder.png");
        m_fileIcon = Texture2D::create("editor/icons/file.png");
    }

    void AssetManagerPannel::onImGui() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
		ImGui::Begin("Assets");

        if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Right) &&
            !ImGui::IsAnyItemHovered()) {
            DEER_CORE_INFO("Clicked");
            ImGui::OpenPopup("AssetManagerPopUp");
        }
        updateContextMenu();

        float width = ImGui::GetWindowContentRegionWidth();
        int cols = (int)ceilf(width / (m_iconMinSize + 20));

        if (m_currentPath != "assets") {
            if (ImGui::Button(m_currentPath.parent_path().string().c_str()))
                m_currentPath = m_currentPath.parent_path();
        }
        else
            ImGui::Button("assets");

        try {
            ImGui::Columns(cols, 0, false);
            for (const auto& entry : fs::directory_iterator(m_currentPath)) {

                if (entry.is_directory())
                    drawFolder(entry.path());
                else
                    drawFile(entry.path());

                ImGui::NextColumn();
            }
            ImGui::Columns();
        }
        catch (const fs::filesystem_error& err) {
            DEER_CORE_ERROR("Asset manager filesystem error");
        }


		ImGui::End();
        ImGui::PopStyleVar();
	}

    void AssetManagerPannel::drawFolder(std::filesystem::path path) {
        ImGui::Image((void*)m_folderIcon->getTextureID(), ImVec2(m_iconMinSize , m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));
        
        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
            m_currentPath = path;
        }
        
        ImGui::Text(path.filename().string().c_str());
    }

    void AssetManagerPannel::updateContextMenu() {

        if (ImGui::BeginPopup("AssetManagerPopUp")) {
            if (ImGui::MenuItem("New Folder")) {

                std::filesystem::path path = m_currentPath / "newFolder";
                std::filesystem::create_directory(path);
            }
            
            ImGui::EndPopup();
        }

    }

    void AssetManagerPannel::drawFile(std::filesystem::path path) {
        
        std::string extension = path.filename().extension().string();

        if (extension == ".dscn") {

        }
        else {
            ImGui::Image((void*)m_fileIcon->getTextureID(), ImVec2(m_iconMinSize, m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));

        }

        ImGui::Text(path.filename().string().c_str());

    }
}
