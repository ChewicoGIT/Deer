#include "Application.h"
#include "Deer/Core/Log.h"

#ifndef DEER_SERVICE
#include "DeerRender/Render/RenderCommand.h"
#include "DeerRender/Render/Render.h"
#include "imgui.h"

#include <functional>
#endif

namespace Deer {
	Application* Application::s_application;

	Application::Application(const WindowProps& props) 
        : m_running(false) {
#ifndef DEER_SERVICE
        m_window = Scope<Window>(Window::create(props));
        m_window->setEventCallback(std::bind(&Application::onEventCallback, this, std::placeholders::_1));
#endif
	}

	void Application::run() {
        s_application = this;
        m_running = true;

        const double targetUpdateTime = 1.0 / 60.0;  // Fixed 60 FPS update
        double targetRenderTime = 1.0 / 120.0;  // User-defined render FPS

        auto previousTime = std::chrono::high_resolution_clock::now();
        double accumulatedUpdateTime = 0.0;
        double accumulatedRenderTime = 0.0;

#ifndef DEER_SERVICE
        m_imGuiLayer.onAttach();
#endif
        onInit();

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

#ifndef DEER_SERVICE
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

#ifndef DEER_SERVICE
        m_imGuiLayer.onDetach();
#endif
        onShutdown();
	}

#ifndef DEER_SERVICE
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
