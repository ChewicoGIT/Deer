#include "DeerStudio.h"

#include "DeerStudio/Editor/PropertiesPannel.h"
#include "DeerStudio/Editor/AssetManagerPannel.h"
#include "DeerStudio/Editor/GamePannel.h"

#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/SceneDataStore.h"
#include "Deer/Scripting/ScriptEngine.h"

#include "Deer/DataStore/DataAccess/PhyisicalDataAccess.h"

#include "Deer/DataStore/DataStore.h"

#include "Editor/SceneExplorer.h"
#include "Editor/TreePannel.h"
#include "Editor/Viewport.h"
#include "Editor/TerrainEditor.h"

#include "Style.h"
#include "Editor/Icons.h"

#include "Plattform/OpenGL/imgui_impl_opengl3.h"

#include <functional>

namespace Deer {
    void DeerStudioApplication::onInit() {
        Path projectPath = Application::m_window->folderDialog(nullptr);
        Project::m_scriptEngine->compileScriptEngine(projectPath / std::filesystem::path("scripts"));

        DataStore::rootPath = projectPath;
        DataStore::setupDataAccess(new PhyisicalDataAccess());

        setupIcons();

        // IMGUI STYLE
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        
        std::string fLoc = (projectPath / "imgui.ini").generic_string();
        char* filenameFLoc = new char[fLoc.size() + 1]();
        strcpy(filenameFLoc, fLoc.c_str());
        io.IniFilename = filenameFLoc;
        ImFontConfig cnfg;
        //cnfg.SizePixels = 26
        Path rfPath = projectPath / "editor/fonts/Roboto-Regular.ttf";
        io.Fonts->AddFontFromFileTTF(rfPath.generic_string().c_str(), 18);
        io.Fonts->AddFontDefault(&cnfg);
    
        
        ImGui_ImplOpenGL3_CreateFontsTexture();
        setNatureStyle();

        auto m_propertiesPannel = Ref<PropertiesPannel>(new PropertiesPannel());
        auto m_assetPannel = Ref<AssetManagerPannel>(new AssetManagerPannel());
        auto m_gamePannel = Ref<GamePannel>(new GamePannel());

        pannels.push_back(m_propertiesPannel);
        pannels.push_back(m_assetPannel);
        pannels.push_back(m_gamePannel);

    }

    void DeerStudioApplication::onShutdown() {
        if (Project::m_scene.getExecutingState())
            Project::m_scene.endExecution();

        Project::m_scriptEngine->shutdownScriptEngine();
        pannels.clear();
    }

    void DeerStudioApplication::onRender(Timestep delta) {
        for (auto pannel : pannels) {
            pannel->onRender(delta);
        }

        int windowWidth = Application::s_application->m_window->getWitdth();
        int windowHeight = Application::s_application->m_window->getHeight();
    }

    void DeerStudioApplication::onUpdate(Timestep delta) {
        if (Project::m_scene.getExecutingState())
            Project::m_scene.updateInternalVars();
        if (Project::m_scene.getVoxelWorld())
            Project::m_scene.getVoxelWorld()->bakeNextChunk();
    }

    void DeerStudioApplication::onEvent(Event& e) {
        for (auto& pannel : pannels)
            pannel->onEventCallback(e);

        viewport_onEvent(e);
    }

    void DeerStudioApplication::onImGUI() {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            window_flags |= ImGuiWindowFlags_NoBackground;
        }

        static bool p_open = true;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("DockSpace Demo", &p_open, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("DockSpace Demo");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        if (ImGui::BeginMenuBar()) {
            drawMenuBar();
            ImGui::EndMenuBar();
        }

        for (auto pannel : pannels) {
            pannel->onImGui();
        }

        // ---- PANNELS -----
        sceneExplorer_onImGUI();
        treePannel_onImGui();
        terrainEditor_onImGui();
        viewport_onImGui();
        // ---- PANNELS -----

        ImGui::End();
    }

    void DeerStudioApplication::drawMenuBar() {
        if (ImGui::BeginMenu("Project")) {
            if (ImGui::MenuItem("New project")) {
                // TODO
            }
            if (ImGui::MenuItem("Open project")) {
                // TODO
            }
            if (ImGui::MenuItem("Save project")) {
                // TODO
            }
            if (ImGui::MenuItem("Save project as...")) {
                // TODO
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Create binaries")) {

            }
            if (ImGui::MenuItem("Export project")) {
                SceneDataStore::exportScenesBin();

                std::vector<Path> scenes = DataStore::getFiles("scenes", ".dbscn");
                DataStore::compressFiles(scenes, "bin/scene_data");
            }
            if (ImGui::MenuItem("Project settings")) {
                // TODO
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("New scene")) {
                // TODO
                Project::m_scene.clear();
                SceneDataStore::exportSceneJson(Project::m_scene, "new_scene");
            }
            //if (Project::m_sceneSerializer->getCurrentScenePath() != "_NO_INITIALIZED_" && ImGui::MenuItem("Save scene")) {
            //    Project::m_sceneSerializer->serialize(Project::m_sceneSerializer->getCurrentScenePath());
            //}
            if (ImGui::MenuItem("Save scene")) {
                SceneDataStore::exportSceneJson(Project::m_scene, "saved_scene");
            }
            if (ImGui::MenuItem("Save scene as...")) {
                // TODO
            }
            if (ImGui::MenuItem("Load scene")) {
                //Project::m_scene.swap(SceneDataStore::loadScene("new_scene"));
                //SceneDataStore::exportSceneJson(Project::m_scene, "new_scene");
            }
            if (ImGui::MenuItem("Scene settings")) {
                // TODO
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Runtime")) {
            if (ImGui::MenuItem("Start")) {
                // TODO
            }
            if (ImGui::MenuItem("End")) {
                // TODO
            }
            if (ImGui::MenuItem("Restart")) {
                // TODO
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Voxel")) {
            if (ImGui::MenuItem("Voxel Pannel")) {
                // TODO
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scripts")) {
            if (ImGui::MenuItem("Reload scripts") && !Project::m_scene.getExecutingState()) {
                Project::m_scriptEngine->shutdownScriptEngine();
                Project::m_scriptEngine->compileScriptEngine(std::filesystem::path("scripts"));
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Documentation")) {
                // TODO
            }
            if (ImGui::MenuItem("Report bug")) {
                // TODO
            }
            if (ImGui::MenuItem("About")) {
                // TODO
            }
            ImGui::EndMenu();
        }

    }

    void DeerStudioApplication::onChangeScene() {
        ActiveEntity::clear();
    }
}