#include "DeerStudio.h"

#include "DeerStudio/Editor/EnviromentTreePannel.h"
#include "DeerStudio/Editor/PropertiesPannel.h"
#include "DeerStudio/Editor/ViewportPannel.h"
#include "DeerStudio/Editor/AssetManagerPannel.h"
#include "DeerStudio/Editor/GamePannel.h"

#include "Deer/Core/Project.h"
#include "Deer/Scene/Scene.h"
#include "Deer/Scene/SceneSerializer.h"
#include "Deer/Scripting/ScriptEngine.h"

#include "Style.h"
#include "Plattform/OpenGL/imgui_impl_opengl3.h"

namespace Deer {
    void DeerStudioApplication::onInit() {
        Project::m_scriptEngine->initScriptEngine(std::filesystem::path("scripts"));

        // IMGUI STYLE
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        ImFont* font1 = io.Fonts->AddFontFromFileTTF("editor\\fonts\\OpenSans-VariableFont_wdth,wght.ttf", 19.0f);
        ImGui_ImplOpenGL3_CreateFontsTexture();
        setNatureStyle();

        // WINDOWS
        m_activeEntity = Ref<ActiveEntity>(new ActiveEntity());

        auto m_propertiesPannel = Ref<PropertiesPannel>(new PropertiesPannel(m_activeEntity));
        auto m_viewportPannel = Ref<ViewportPannel>(new ViewportPannel(Project::m_scene->getMainEnviroment(), "Scene viewport", m_activeEntity));
        auto m_enviromentTreePannel = Ref<EnviromentTreePannel>(new EnviromentTreePannel(Project::m_scene->getMainEnviroment(), "World tree", m_activeEntity));
        auto m_assetPannel = Ref<AssetManagerPannel>(new AssetManagerPannel(m_activeEntity));
        auto m_gamePannel = Ref<GamePannel>(new GamePannel(m_activeEntity));

        pannels.push_back(m_propertiesPannel);
        pannels.push_back(m_enviromentTreePannel);
        pannels.push_back(m_viewportPannel);
        pannels.push_back(m_assetPannel);
        pannels.push_back(m_gamePannel);
    }

    void DeerStudioApplication::onShutdown() {
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
        if (ImGui::BeginMenu("Project")) {

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("New scene")) {
                m_activeEntity->clear();
                Project::m_scene->clear();
                Project::m_sceneSerializer->serialize("assets/newScene.dscn");
            }

            if (Project::m_sceneSerializer->getCurrentScenePath() != "_NO_INITIALIZED_" && ImGui::MenuItem("Save scene")) {
                Project::m_sceneSerializer->serialize(Project::m_sceneSerializer->getCurrentScenePath());
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Scripts")) {
            if (ImGui::MenuItem("Reload scripts") && !Project::m_scene->getExecutingState()) {
                Project::m_scriptEngine->shutdownScriptEngine();
                Project::m_scriptEngine->initScriptEngine(std::filesystem::path("scripts"));
            }

            ImGui::EndMenu();
        }
    }
}