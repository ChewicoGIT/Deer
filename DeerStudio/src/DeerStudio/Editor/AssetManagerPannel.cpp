#include "AssetManagerPannel.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Project.h"
#include "DeerRender/Render/Texture.h"
#include "Deer/Asset/AssetManager.h"
#include "DeerStudio/Editor/ActiveEntity.h"

#include "Deer/Core/Project.h"

#include "imgui.h"

#include <string>

#ifdef WINDOWS
// WINDOWS SPECIFIC
#include <windows.h>
#include <shellapi.h>
#endif

namespace Deer {
    void openFileExplorer(const std::string& relativePath) {
        #ifdef WINDOWS
        // Convert std::string to std::wstring
        std::wstring widePath(relativePath.begin(), relativePath.end());
        wchar_t buffer[MAX_PATH];
        GetFullPathNameW(widePath.c_str(), MAX_PATH, buffer, nullptr);

        // Open the file explorer at the given path
        ShellExecuteW(nullptr, L"open", buffer, nullptr, nullptr, SW_SHOW);
        #endif
    }

    namespace fs = std::filesystem;

    AssetManagerPannel::AssetManagerPannel()
        : m_currentPath(DataStore::rootPath / "assets"){

        m_folderIcon = Texture2D::create("editor/icons/folder.png");
        m_fileIcon = Texture2D::create("editor/icons/file.png");
        m_scneIcon = Texture2D::create("editor/icons/scene.png");
        m_objectIcon = Texture2D::create("editor/icons/object.png");
        m_shaderIcon = Texture2D::create("editor/icons/shader.png");
    }

    void AssetManagerPannel::onImGui() {
        m_clickHandled = false;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 30));
		ImGui::Begin("Assets");
        ImGui::PopStyleVar();

        float width = ImGui::GetWindowContentRegionWidth();

        if (width < m_iconMinSize + 40) {
            ImGui::Text("Window too small");
            ImGui::End();
            return;
        }

        int cols = (int)ceilf(width / (m_iconMinSize + 40));
        float componentWidth = width / (float)cols;

        if (m_currentPath != "assets") {
            if (ImGui::Button(m_currentPath.parent_path().string().c_str()))
                m_currentPath = m_currentPath.parent_path();
        }
        else
            ImGui::Button("assets");

        ImGui::SameLine();

        if (ImGui::Button("Open File Explorer"))
            openFileExplorer(m_currentPath.string());

        try {
            ImGui::Columns(cols, 0, false);
            for (const auto& entry : fs::directory_iterator(m_currentPath)) {
                if (entry.is_directory())
                    drawFolder(entry.path());
                else
                    drawFile(entry.path());

                float cursorOffset = (m_iconMinSize - ImGui::CalcTextSize(entry.path().filename().string().c_str()).x) / 2;
                ImGui::SetCursorPos(ImVec2(cursorOffset + ImGui::GetCursorPos().x, ImGui::GetCursorPos().y));
                ImGui::Text("%s",entry.path().filename().string().c_str());

                ImGui::NextColumn();
            }
            ImGui::Columns();
        }
        catch (const fs::filesystem_error& err) {
            DEER_CORE_ERROR("Asset manager filesystem error");
        }


        if (!m_clickHandled && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
            ImGui::IsMouseClicked(ImGuiMouseButton_Right) &&
            !ImGui::IsAnyItemHovered()) {

            ImGui::OpenPopup("AssetManagerPopUp");
        }
        updateContextMenu();

		ImGui::End();
	}

    void AssetManagerPannel::drawFolder(const std::filesystem::path& path) {
        ImGui::Image((void*)(uint64_t)m_folderIcon->getTextureID(), ImVec2(m_iconMinSize , m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));
        
        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
            m_currentPath = path;
        }
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

    void AssetManagerPannel::drawFile(const std::filesystem::path& path) {
        std::string extension = path.filename().extension().string();

        if (extension == ".dscn") {
            ImGui::Image((void*)(uint64_t)m_scneIcon->getTextureID(), ImVec2(m_iconMinSize, m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));

            // Open scene
            if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
                try {
                    m_currentScenePath = path;
                }
                catch ( ... ) {
                    DEER_CORE_ERROR("Error while loading scene {0}, file might be corrupt", path.string().c_str());
                }
            }
        } else if (extension == ".obj") {
            ImGui::Image((void*)(uint64_t)m_objectIcon->getTextureID(), ImVec2(m_iconMinSize, m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                std::string pathString = path.string();
                ImGui::SetDragDropPayload("_MESH", pathString.c_str(), pathString.size() + 1);
                ImGui::Text("%s", path.string().c_str());
                ImGui::EndDragDropSource();
            }
        } else if (extension == ".glsl") {
            ImGui::Image((void*)(uint64_t)m_shaderIcon->getTextureID(), ImVec2(m_iconMinSize, m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                std::string pathString = path.string();
                ImGui::SetDragDropPayload("_SHADER", pathString.c_str(), pathString.size() + 1);
                ImGui::Text("%s", path.string().c_str());
                ImGui::EndDragDropSource();
            }
        } else if (extension == ".png" || extension == ".jpg" || extension == ".jpeg") {
            uid textureID = AssetManager::loadAsset<Texture2D>(path.string());
            Asset<Texture2D>& textureAsset = AssetManager::getAsset<Texture2D>(textureID);

            ImGui::Image((void*)(uint64_t)textureAsset.value->getTextureID(), ImVec2(m_iconMinSize, m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));
        
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                std::string pathString = path.string();
                ImGui::SetDragDropPayload("_TEXTURE2D", pathString.c_str(), pathString.size() + 1);
                ImGui::Text("%s", path.string().c_str());
                ImGui::EndDragDropSource();
            }
        } else {
            ImGui::Image((void*)(uint64_t)m_fileIcon->getTextureID(), ImVec2(m_iconMinSize, m_iconMinSize), ImVec2(0, 1), ImVec2(1, 0));
        }

    }
}
