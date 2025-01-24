#include "SceneExplorer.h"
#include "imgui.h"
#include "Icons.h"

#include "Deer/DataStore/Path.h"
#include "Deer/DataStore/DataStore.h"
#include "DeerRender/Render/Texture.h"
#include "Deer/Core/Project.h"
#include "Deer/Scene/SceneDataStore.h"
#include "Deer/Scene/Scene.h"

namespace Deer {
    Path m_currentScenePath(DEER_SCENE_PATH);

    void drawSceneExplorerFolder(const Path& path);
    void drawSceneExplorerScene(const Path& path);

	void sceneExplorer_onImGUI() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(30, 30));
		ImGui::Begin("Scene Explorer", (bool*)0, ImGuiWindowFlags_MenuBar);
        ImGui::PopStyleVar();


        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("Save Scene")) {

            }

            if (ImGui::MenuItem("New Scene")) {

            }

            ImGui::EndMenuBar();
        }

        float width = ImGui::GetWindowContentRegionWidth();

        if (width < ICON_MIN_SIZE + 40) {
            ImGui::End();
            return;
        }

        int cols = (int)ceilf(width / (ICON_MIN_SIZE + 40));
        float componentWidth = width / (float)cols;

        if (m_currentScenePath != DEER_SCENE_PATH) {
            if (ImGui::Button(m_currentScenePath.parent_path().string().c_str()))
                m_currentScenePath = m_currentScenePath.parent_path();
        }
        else
            ImGui::Button(DEER_SCENE_PATH);

        ImGui::Columns(cols, 0, false);
        
        for (const auto& entry : std::filesystem::directory_iterator(m_currentScenePath)) {
            

            if (entry.is_directory())
                drawSceneExplorerFolder(entry.path());
            else {
                std::string extension = entry.path().filename().extension().string();
                if (extension != ".dscn")
                    continue;
            }
                drawSceneExplorerScene(entry.path());

            float cursorOffset = (ICON_MIN_SIZE - ImGui::CalcTextSize(entry.path().filename().string().c_str()).x) / 2;
            ImGui::SetCursorPos(ImVec2(cursorOffset + ImGui::GetCursorPos().x, ImGui::GetCursorPos().y));
            ImGui::Text(entry.path().filename().string().c_str());

            ImGui::NextColumn();
        }
        ImGui::Columns();

		ImGui::End();
	}

    void drawSceneExplorerFolder(const Path& path) {
        ImGui::Image((void*)folder_icon->getTextureID(), ImVec2(ICON_MIN_SIZE, ICON_MAX_SIZE), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
            m_currentScenePath = path;
        }
    }


    void drawSceneExplorerScene(const Path& path) {
        ImGui::Image((void*)scene_icon->getTextureID(), ImVec2(ICON_MIN_SIZE, ICON_MIN_SIZE), ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::IsItemClicked(0) && ImGui::IsMouseDoubleClicked(0)) {
            Project::m_scene = SceneDataStore::loadScene(path);
        }
    }
}