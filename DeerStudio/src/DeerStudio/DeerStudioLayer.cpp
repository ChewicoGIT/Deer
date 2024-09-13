#include "DeerStudioLayer.h"
#include "imgui.h"
#include "Deer/Render/Render.h"
#include "Deer/Core/Window.h"
#include "Deer/Core/Log.h"
#include "DeerStudio/Editor/ViewportPannel.h"
#include "backends/imgui_impl_opengl3.h"
#include "Style.h"

const float toolbarSize = 50;
namespace Deer {
    void DeerStudioLayer::onAttach() {
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear(); 
        setNatureStyle();

        ImFont* font1 = io.Fonts->AddFontFromFileTTF("C:\\Chewico\\Projects\\Deer\\DeerStudio\\editor\\fonts\\OpenSans-VariableFont_wdth,wght.ttf", 20.0f);
        //ImGui::PushFont(font1);
        ImGui_ImplOpenGL3_CreateFontsTexture();

        m_enviroment = Ref<Environment>(new Environment());
        m_propertiesPannel = Ref<PropertiesPannel>(new PropertiesPannel());
        auto m_enviromentTreePannel = Ref<EnviromentTreePannel>(new EnviromentTreePannel(m_enviroment, "World tree"));
        
        m_enviromentTreePannel->setActiveEntity(&m_activeEntity);

        m_vertexArray = VertexArray::create();
        m_camera = Scope<Camera>(new Camera(16 / 9, 50, 0.05f, 100));
        m_texture = Texture2D::create("assets/test_texture.png");

        FrameBufferSpecification bufferSpecs(Application::s_application->m_window->getWitdth(),
            Application::s_application->m_window->getHeight());
        DEER_CORE_INFO(Application::s_application->m_window->getHeight());

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f
        };

        m_vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices));

        BufferLayout bufferLayout({
            {"a_Position", DataType::Float3, ShaderDataType::FloatingPoint },
            {"a_Color", DataType::Float2, ShaderDataType::FloatingPoint },
            });

        m_vertexBuffer->setLayout(bufferLayout);
        m_vertexArray->addVertexBuffer(m_vertexBuffer);

        unsigned int indices[] = {
            0, 2, 1,
            1, 2, 3
        };

        m_indexBuffer = IndexBuffer::create(indices, sizeof(indices), IndexDataType::Unsigned_Int);
        m_vertexArray->setIndexBuffer(m_indexBuffer);

        std::string vertexSource = R"(
			#version 410 core	
			layout(location = 0) in vec3 a_position;
			layout(location = 1) in vec2 a_uv;

            out vec2 textCoord;
			
			uniform mat4 u_viewMatrix;
			uniform mat4 u_worldMatrix;
			void main()
			{
				//gl_Position = vec4(a_position, 1.0) * u_projectionMatrix * u_viewMatrix;
				gl_Position = u_viewMatrix * u_worldMatrix * vec4(a_position,1.0);

                textCoord = a_uv;
			}
		)";

        std::string fragmentSrc = R"(
			#version 410 core	
			
            out vec4 fragColor;
            in vec2 textCoord;

            uniform sampler2D u_texture;

			void main()
			{
				fragColor = texture(u_texture, textCoord);
                //fragColor = vec4(textCoord, 0, 1);
			}
		)";

        m_shader = Shader::create(vertexSource, fragmentSrc);
        MeshRenderComponent& mrc = m_enviroment->createEntity("Square").addComponent<MeshRenderComponent>();
        mrc.mesh = m_vertexArray;
        mrc.shader = m_shader;

        Entity camEntity = m_enviroment->createEntity("Camera");
        camEntity.addComponent<CameraComponent>();
        camEntity.getComponent<TransformComponent>().position = glm::vec3(0, 0, -3);

        m_viewportPannel = Ref<ViewportPannel>(new ViewportPannel(m_enviroment, "Scene viewport"));

        pannels.push_back(m_propertiesPannel);
        pannels.push_back(m_enviromentTreePannel);
        pannels.push_back(m_viewportPannel);

        m_enviroment->setMainCamera(camEntity);
    }

    void DeerStudioLayer::onUpdate(Timestep delta) {
        for (auto pannel : pannels) {
            pannel->onUpdate(delta);
        }

        m_texture->bind(0);
        m_shader->uploadUniformInt("u_texture", 0);


        int windowWidth = Application::s_application->m_window->getWitdth();
        int windowHeight = Application::s_application->m_window->getHeight();

        m_camera->setAspect((float)windowWidth / (float)windowHeight);
        m_camera->setPosition(pos);
        m_camera->setRotation(glm::quat(rotation));
        m_camera->recalculateMatrices();
    }

    void DeerStudioLayer::onEvent(Event& e) {

    }

    void DeerStudioLayer::onImGUI() {

        m_viewportPannel->m_entity = m_activeEntity;

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

        m_propertiesPannel->setEntity(m_activeEntity);
        for (auto pannel : pannels) {
            pannel->onImGui();
        }

        drawWindows();
        ImGui::End();
	}

    void DeerStudioLayer::drawMenuBar() {

        if (ImGui::BeginMenu("Project")) {
            //ImGui::MenuItem("New project");
            //ImGui::MenuItem("Open project");

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {

            ImGui::EndMenu();
        }

    }
    void DeerStudioLayer::drawWindows() {

        ImGui::Begin("Debug pos");
        ImGui::DragFloat3("camPos", &pos.x, 0.01f);
        ImGui::DragFloat3("camRotation", &rotation.x, 0.01f);
        ImGui::Spacing();
        ImGui::DragFloat("fov", &fov, 0.07f);
        ImGui::Spacing();
        ImGui::DragFloat3("objectPos", &m_transform.position.x, 0.01f);
        ImGui::DragFloat3("objectRotation", &objectRotation.x, 0.01f);
        ImGui::DragFloat3("objectScale", &m_transform.scale.x, 0.01f);

        m_transform.setEulerAngles(objectRotation);

        m_camera->setFov(fov);
        m_camera->setPosition(pos);
        m_camera->setRotation(glm::quat(rotation));
        m_camera->recalculateMatrices();
        ImGui::End();
    }
}
