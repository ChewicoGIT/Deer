#include "Application.h"
#include "Deer/Core/Log.h"

#ifdef DEER_RENDER
#include "DeerRender/Render/RenderCommand.h"
#include "DeerRender/Render/Render.h"
#include "DeerRender/Render/RenderUtils.h"
#include "imgui.h"

#include <functional>
#endif

namespace Deer {
	Application* Application::s_application;

    Application::Application() : m_running(false) {
#ifdef DEER_RENDER
        m_window = Scope<Window>(Window::create(m_windowProps));
#endif
    }

#ifdef DEER_RENDER
    Application::Application(const WindowProps& props)
        : m_running(false), m_windowProps(props) {
        m_window = Scope<Window>(Window::create(m_windowProps));
	}

    void Application::initializeWindow() {
        m_window->initWindow();
        m_window->setEventCallback(std::bind(&Application::onEventCallback, this, std::placeholders::_1));
    }
#endif

	int Application::run() {
        s_application = this;
        m_running = true;

        const double targetUpdateTime = 1.0 / 60.0;  // Fixed 60 FPS update
        double targetRenderTime = 1.0 / 120.0;  // User-defined render FPS

        auto previousTime = std::chrono::high_resolution_clock::now();
        double accumulatedUpdateTime = 0.0;
        double accumulatedRenderTime = 0.0;

        int res = onPreInit();
        if (res != 0)
            return res;

#ifdef DEER_RENDER
        initializeWindow();
        m_imGuiLayer.onAttach();
		RenderUtils::initializeRenderUtils();
		RenderCommand::init();
#endif

        res = onInit();
        if (res != 0){

#ifdef DEER_RENDER
            m_imGuiLayer.onDetach();
#endif
            return res;
        }

        while (m_running) {
            // Time handling
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> deltaTime = currentTime - previousTime;
            previousTime = currentTime;

            accumulatedUpdateTime += deltaTime.count();
            accumulatedRenderTime += deltaTime.count();

            // Fixed Update loop (60 FPS)
            while (accumulatedUpdateTime >= targetUpdateTime) {
                Timestep timestep = (float)targetUpdateTime;
                onUpdate(timestep);
                accumulatedUpdateTime -= targetUpdateTime;
            }

#ifdef DEER_RENDER
            // Render loop (User-defined FPS)
            if (accumulatedRenderTime >= targetRenderTime) {
                RenderCommand::setClearColor({ 0.2f, 0.2f, 0.3f, 1.0f });
                RenderCommand::clear();

                Render::beginExecution();
                onRender(Timestep((float)targetRenderTime));
                Render::endExecution();

                ImGuiIO& io = ImGui::GetIO();
                io.DeltaTime = (float)targetRenderTime;
                m_imGuiLayer.begin();
                onImGUI();
                m_imGuiLayer.end();

                accumulatedRenderTime -= targetRenderTime;

                m_window->onRender();
            }
#endif

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

#ifdef DEER_RENDER
        m_imGuiLayer.onDetach();
#endif
        onShutdown();
        return 0;
	}

#ifdef DEER_RENDER
    void Application::onEventCallback(Event& e) {
        onEvent(e);
        m_imGuiLayer.onEvent(e);

        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
    }

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        m_running = false;
        return true;
    }
#endif
}
