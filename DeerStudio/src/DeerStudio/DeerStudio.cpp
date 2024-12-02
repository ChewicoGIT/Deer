#include "DeerStudio.h"

#include "DeerStudio/Editor/EnviromentTreePannel.h"
#include "DeerStudio/Editor/PropertiesPannel.h"
#include "DeerStudio/Editor/ViewportPannel.h"
#include "DeerStudio/Editor/AssetManagerPannel.h"
#include "DeerStudio/Editor/GamePannel.h"
#include "DeerStudio/Editor/TerrainEditorPannel.h"

#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/SceneSerializer.h"
#include "Deer/Scripting/ScriptEngine.h"

#include "Style.h"
#include "Plattform/OpenGL/imgui_impl_opengl3.h"

#include <functional>

namespace Deer {
    void DeerStudioApplication::onInit() {
        Project::m_scriptEngine->compileScriptEngine(std::filesystem::path("scripts"));
        Project::m_sceneSerializer->setSceneChangeCallback(std::bind(&Deer::DeerStudioApplication::onChangeScene, this));

        // IMGUI STYLE
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        ImFont* font1 = io.Fonts->AddFontFromFileTTF("editor\\fonts\\OpenSans-VariableFont_wdth,wght.ttf", 19.0f);
        ImGui_ImplOpenGL3_CreateFontsTexture();
        setNatureStyle();

        // WINDOWS
        m_activeEntity = Ref<ActiveEntity>(new ActiveEntity());
        VoxelWorldProps worldProps(2, 2, 2);
        Project::m_scene->createVoxelWorld(worldProps);

        auto m_propertiesPannel = Ref<PropertiesPannel>(new PropertiesPannel(m_activeEntity));
        auto m_viewportPannel = Ref<ViewportPannel>(new ViewportPannel("Scene viewport", m_activeEntity));
        auto m_enviromentTreePannel = Ref<EnviromentTreePannel>(new EnviromentTreePannel(Project::m_scene->getMainEnviroment(), "World tree", m_activeEntity));
        auto m_assetPannel = Ref<AssetManagerPannel>(new AssetManagerPannel(m_activeEntity));
        auto m_gamePannel = Ref<GamePannel>(new GamePannel(m_activeEntity));
        auto m_terrainEditor = Ref<TerrainEditorPannel>(new TerrainEditorPannel());

        pannels.push_back(m_propertiesPannel);
        pannels.push_back(m_enviromentTreePannel);
        pannels.push_back(m_viewportPannel);
        pannels.push_back(m_assetPannel);
        pannels.push_back(m_gamePannel);
        pannels.push_back(m_terrainEditor);
    }

    void DeerStudioApplication::onShutdown() {
        if (Project::m_scene->getExecutingState())
            Project::m_scene->endExecution();

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
        if (Project::m_scene->getExecutingState())
            Project::m_scene->updateExecution();
    }

    void DeerStudioApplication::onEvent(Event& e) {
        for (auto& pannel : pannels)
            pannel->onEventCallback(e);
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

        ImGui::End();
    }

    void DeerStudioApplication::drawMenuBar() {
        if (ImGui::BeginMenu("Archive")) {
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
            if (ImGui::MenuItem("Export project")) {
                // TODO
            }
            if (ImGui::MenuItem("Project settings")) {
                // TODO
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Scene")) {
            if (ImGui::MenuItem("New scene")) {
                // TODO
                Project::m_scene->clear();
                Project::m_sceneSerializer->serialize("assets/newScene.dscn");
            }
            //if (Project::m_sceneSerializer->getCurrentScenePath() != "_NO_INITIALIZED_" && ImGui::MenuItem("Save scene")) {
            //    Project::m_sceneSerializer->serialize(Project::m_sceneSerializer->getCurrentScenePath());
            //}
            if (ImGui::MenuItem("Save scene")) {
                // TODO
            }
            if (ImGui::MenuItem("Save scene as...")) {
                // TODO
            }
            if (ImGui::MenuItem("Load scene")) {
                // TODO
            }
            if (ImGui::MenuItem("Scene settings")) {
                // TODO
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
                // TODO
            }
            if (ImGui::MenuItem("Copy")) {
                // TODO
            }
            if (ImGui::MenuItem("Paste")) {
                // TODO
            }
            if (ImGui::MenuItem("Duplicate")) {
                // TODO
            }
            if (ImGui::MenuItem("Select all")) {
                // TODO
            }
            if (ImGui::MenuItem("Deselect all")) {
                // TODO
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Zoom In")) {
                // TODO
            }
            if (ImGui::MenuItem("Zoom Out")) {
                // TODO
            }
            if (ImGui::MenuItem("Show Gizmo/ Hide Gizmo")) {
                // TODO
            }
            if (ImGui::MenuItem("Full screen")) {
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

        if (ImGui::BeginMenu("Scripts")) {
            if (ImGui::MenuItem("Reload scripts") && !Project::m_scene->getExecutingState()) {
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
        m_activeEntity->clear();
    }
}