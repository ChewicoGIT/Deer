#include "SceneExplorer.h"
#include "imgui.h"
#include "Icons.h"

#include "Deer/DataStore/Path.h"
#include "Deer/DataStore/DataStore.h"
#include "DeerRender/Render/Texture.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/SceneDataStore.h"
#include "Deer/Scene/Scene.h"
#include "DeerStudio/Editor/ActiveEntity.h"
#include "Deer/Core/Log.h"

#include "EditorUtils.h"

#include <regex>
#include <string>

#ifdef WINDOWS
// WINDOWS SPECIFIC
#include <windows.h>
#include <shellapi.h>
#endif

namespace Deer {
    Path m_currentScenePath(DEER_SCENE_PATH);
    Path m_currentSceneName;
    Path m_loadSceneName;
    Path m_deleteSceneName;
    Path m_dialogSceneName;

    void drawSceneExplorerFolder(const Path& path);
    void drawSceneExplorerScene(const Path& path);
    void saveSceneBeforeLoadingPopup();
    void sceneDialogPopup();

    void saveSceneName(const std::string&);
    void createFolderName(const std::string& name);
    void saveBeforeCreatingNew(bool save);
    void saveSceneNameBeforeCreatingNew(const std::string& name);
    void deleteScene();

    void openFileExplorer(const std::string& relativePath);

	void sceneExplorer_onImGUI() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 10));
		ImGui::Begin("Scene Explorer", (bool*)0, ImGuiWindowFlags_MenuBar);
        ImGui::PopStyleVar();

        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("Save")) {
                if (m_currentSceneName == "")
                    ImGui::OpenPopup("SAVE_SCENE_NAME");
                else
                    SceneDataStore::exportSceneJson(Project::m_scene, m_currentSceneName);
            }

            if (ImGui::MenuItem("Save as")) {
                ImGui::OpenPopup("SAVE_SCENE_NAME");
            }
            if (ImGui::MenuItem("New Scene")) {
                ImGui::OpenPopup("SAVE_SCENE_BEFORE_CREATING_NEW");
            }

            if (ImGui::MenuItem("New folder")) {
                ImGui::OpenPopup("CREATE_SCENE_FOLDER");
            }

            if (ImGui::MenuItem("Explorer")) {
                openFileExplorer(m_currentScenePath.generic_string());
            }

            stringInputPopup<saveSceneName>("SAVE_SCENE_NAME", "Scene name");
            stringInputPopup<createFolderName>("CREATE_SCENE_FOLDER", "Folder name");
            saveInputPopup<saveBeforeCreatingNew>("SAVE_SCENE_BEFORE_CREATING_NEW", "Do you want to save the scene before creating new?");
            stringInputPopup<saveSceneNameBeforeCreatingNew>("SAVE_SCENE_NAME_CREATE_NEW", "Scene name");

            ImGui::EndMenuBar();
        }

        ImGui::Text(m_currentScenePath.generic_string().c_str());

        ImGui::TextDisabled("Active Scene : ");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.7f, 1), m_currentSceneName.generic_string().c_str());

        float width = ImGui::GetWindowContentRegionWidth();

        if (width < ICON_MIN_SIZE + 80) {
            ImGui::End();
            return;
        }

        int cols = (int)ceilf(width / (ICON_MIN_SIZE + 80));
        float componentWidth = width / (float)cols;

        ImGui::Columns(cols, 0, false);

        if (m_currentScenePath != DEER_SCENE_PATH) {
            drawSceneExplorerFolder("..");

            float cursorOffset = (ICON_MIN_SIZE - ImGui::CalcTextSize("..").x) / 2;
            ImGui::SetCursorPos(ImVec2(cursorOffset + ImGui::GetCursorPos().x, ImGui::GetCursorPos().y));
            ImGui::Text("..");

            ImGui::NextColumn();
        }
        
        for (const auto& entry : std::filesystem::directory_iterator(m_currentScenePath)) {
            if (entry.is_directory())
                drawSceneExplorerFolder(entry.path());
            else {
                std::string extension = entry.path().filename().extension().string();
                if (extension != ".dscn")
                    continue;

                Path sceneName = entry.path().parent_path().lexically_relative(DEER_SCENE_PATH) / entry.path().stem();
                drawSceneExplorerScene(sceneName);
            }

            float cursorOffset = (ICON_MIN_SIZE - ImGui::CalcTextSize(entry.path().stem().string().c_str()).x) / 2;
            ImGui::SetCursorPos(ImVec2(cursorOffset + ImGui::GetCursorPos().x, ImGui::GetCursorPos().y));
            ImGui::Text(entry.path().stem().string().c_str());

            ImGui::NextColumn();
        }
        ImGui::Columns();

        saveSceneBeforeLoadingPopup();
        sceneDialogPopup();
        deleteInputPopup<deleteScene>("DELETE_SCENE", "Are you sure you want to delete the scene?");

		ImGui::End();

	}

    void drawSceneExplorerFolder(const Path& path) {
        ImGui::Image((void*)folder_icon->getTextureID(), ImVec2(ICON_MIN_SIZE, ICON_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
            if (path == "..")
                m_currentScenePath = m_currentScenePath.parent_path();
            else
                m_currentScenePath = path;
        }
        
    }

    void drawSceneExplorerScene(const Path& path) {
        ImGui::Image((void*)scene_icon->getTextureID(), ImVec2(ICON_MIN_SIZE, ICON_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
            ImGui::OpenPopup("SAVE_SCENE_BEFORE_LOADING");
            m_loadSceneName = path;
        }

        if (ImGui::IsItemClicked(1)) {
            ImGui::OpenPopup("SCENE_DIALOG");
            m_dialogSceneName = path;
        }
    }

    std::string sanitizeInput(const std::string& input) {
        std::string sanitized;
        for (char c : input) {
            if (isalnum(c) || c == '_') {
                sanitized += c;
            }
        }
        return sanitized;
    }

    void saveSceneBeforeLoadingPopup() {
        if (ImGui::BeginPopup("SAVE_SCENE_BEFORE_LOADING")) {
            if (m_currentSceneName == "") {
                Project::m_scene = SceneDataStore::loadScene(m_loadSceneName);
                m_currentSceneName = m_loadSceneName;
                ImGui::CloseCurrentPopup();
            }

            ImGui::Text("Do you want to save the current scene?");

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
                ActiveEntity::clear();
                SceneDataStore::exportSceneJson(Project::m_scene, m_currentSceneName);
                Project::m_scene = SceneDataStore::loadScene(m_loadSceneName);
                m_currentSceneName = m_loadSceneName;
                ImGui::CloseCurrentPopup();
            }
            else if (dont_save) {
                ActiveEntity::clear();
                Project::m_scene = SceneDataStore::loadScene(m_loadSceneName);
                m_currentSceneName = m_loadSceneName;
                ImGui::CloseCurrentPopup();
            } if (cancel) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void sceneDialogPopup() {
        
        if (ImGui::BeginPopup("SCENE_DIALOG")) {
            if (ImGui::MenuItem("Rename Scene")) {
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Delete Scene")) {
                ImGui::CloseCurrentPopup();
                ImGui::EndPopup();

                ImGui::OpenPopup("DELETE_SCENE");
                m_deleteSceneName = m_dialogSceneName;
                return;
            }
            ImGui::EndPopup();
        }
    }

    void deleteScene() {
        SceneDataStore::deleteSceneJson(m_deleteSceneName);
    }
    void createFolderName(const std::string& name) {
        std::string correctInput = sanitizeInput(name);

        if (name.size() != 0) {
            DataStore::createFolder(m_currentScenePath / correctInput);
        }
    }
    void saveSceneName(const std::string& name) {
        std::string correctInput = sanitizeInput(name);

        if (name.size() != 0) {
            Path fullName;
            if (m_currentScenePath == DEER_SCENE_PATH)
                fullName = correctInput;
            else
                fullName = m_currentScenePath.lexically_relative(DEER_SCENE_PATH) / correctInput;

            m_currentSceneName = fullName;
            SceneDataStore::exportSceneJson(Project::m_scene, fullName);
        }
    }
    void saveBeforeCreatingNew(bool save) {
        if (save) {
            if (m_currentSceneName == "") {
                ImGui::OpenPopup("SAVE_SCENE_NAME_CREATE_NEW");
                return;
            }
            else {
                ActiveEntity::clear();
                SceneDataStore::exportSceneJson(Project::m_scene, m_currentSceneName);
                Project::m_scene = Scene();
                m_currentSceneName = Path();
            }
        } 
        else {
            ActiveEntity::clear();
            Project::m_scene = Scene();
            m_currentSceneName = Path();
        }
    }
    void saveSceneNameBeforeCreatingNew(const std::string& name) {
        std::string correctInput = sanitizeInput(name);

        if (name.size() != 0) {
            Path fullName;
            if (m_currentScenePath == DEER_SCENE_PATH)
                fullName = correctInput;
            else
                fullName = m_currentScenePath.lexically_relative(DEER_SCENE_PATH) / correctInput;

            SceneDataStore::exportSceneJson(Project::m_scene, fullName);
            ActiveEntity::clear();
            Project::m_scene = Scene();
            m_currentSceneName = Path();
        }
    }
}