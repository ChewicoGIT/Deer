#include "DeerStudioLayer.h"
#include "imgui.h"
#include "Deer/Render/Render.h"
#include "Deer/Core/Window.h"
#include "Deer/Core/Log.h"
#include "DeerStudio/Editor/ViewportPannel.h"
#include "DeerStudio/Editor/ActiveEntity.h"
#include "backends/imgui_impl_opengl3.h"
#include "Style.h"

#include "Deer/Core/Project.h"

const float toolbarSize = 50;
namespace Deer {
    void DeerStudioLayer::onAttach() {
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear(); 
        setNatureStyle();

        ImFont* font1 = io.Fonts->AddFontFromFileTTF("editor\\fonts\\OpenSans-VariableFont_wdth,wght.ttf", 19.0f);
        //ImGui::PushFont(font1);
        ImGui_ImplOpenGL3_CreateFontsTexture();
        m_texture = Texture2D::create("assets/skull.jpg");

        m_meshID = Project::m_assetManager.loadAsset<Mesh>(std::filesystem::path("assets/skull.obj"));
        m_shaderID = Project::m_assetManager.loadAsset<Shader>(std::filesystem::path("assets/Shaders/SimpleShader.glsl"));

        m_activeEntity = Ref<ActiveEntity>(new ActiveEntity());
        m_scene = Ref<Scene>(new Scene());
        m_sceneSerializer = Ref<SceneSerializer>(new SceneSerializer(m_scene));

        auto m_propertiesPannel = Ref<PropertiesPannel>(new PropertiesPannel(m_activeEntity));
        auto m_viewportPannel = Ref<ViewportPannel>(new ViewportPannel(m_scene->getMainEnviroment(), "Scene viewport", m_activeEntity));
        auto m_enviromentTreePannel = Ref<EnviromentTreePannel>(new EnviromentTreePannel(m_scene->getMainEnviroment(), "World tree", m_activeEntity));
        auto m_assetPannel = Ref<AssetManagerPannel>(new AssetManagerPannel(m_sceneSerializer, m_activeEntity));

        pannels.clear();
        pannels.push_back(m_propertiesPannel);
        pannels.push_back(m_enviromentTreePannel);
        pannels.push_back(m_viewportPannel);
        pannels.push_back(m_assetPannel);

        auto& entity = m_scene->getMainEnviroment()->createEntity("Square");
        MeshRenderComponent& mrc = entity.addComponent<MeshRenderComponent>();
        mrc.meshAssetID = m_meshID;
        mrc.shaderAssetID = m_shaderID;

    }

    void DeerStudioLayer::onUpdate(Timestep delta) {
        for (auto pannel : pannels) {
            pannel->onUpdate(delta);
        }

        m_texture->bind(0);
        Asset<Shader>& shaderAsset = Project::m_assetManager.getAsset<Shader>(m_shaderID);
        shaderAsset.value->uploadUniformInt("u_texture", 0);

        int windowWidth = Application::s_application->m_window->getWitdth();
        int windowHeight = Application::s_application->m_window->getHeight();
    }

    void DeerStudioLayer::loadScene() {

    }

    void DeerStudioLayer::unloadScene() {
        m_scene.reset();
        m_sceneSerializer.reset();
        m_activeEntity->clear();
        pannels.clear();
    }

    void DeerStudioLayer::onEvent(Event& e) {
        for (auto& pannel : pannels)
            pannel->onEvent(e);
    }

    void DeerStudioLayer::onImGUI() {

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

    void DeerStudioLayer::drawMenuBar() {

        if (ImGui::BeginMenu("Project")) {

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("New scene")) {
                m_activeEntity->clear();
                m_scene->clear();
                m_sceneSerializer->serialize("assets/newScene.dscn");
            }

            if (m_sceneSerializer->getCurrentScenePath() != "_NO_INITIALIZED_" && ImGui::MenuItem("Save scene")) {
                m_sceneSerializer->serialize(m_sceneSerializer->getCurrentScenePath());
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {

            ImGui::EndMenu();
        }

    }
}
