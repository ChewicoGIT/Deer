#include "Application.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Timestep.h"

#include "Deer/Render/RenderCommand.h"
#include "Deer/Render/Render.h"

#include <functional>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Deer {
	Application* Application::s_application;

	Application::Application(const WindowProps& props) {
		m_window = Scope<Window>(Window::create(props));
		m_window->setEventCallback(std::bind(&Application::onEvent, this, std::placeholders::_1));
	}

	void Application::pushLayer(Layer* layer) {
		m_layerStack.pushLayer(layer);
		if(m_running)
			layer->onAttach();
	}

	void Application::onEvent(Event& e) {
		for (auto it = m_layerStack.end(); it != m_layerStack.begin();) {
			(*--it)->onEvent(e);
		}
		m_imGuiLayer.onEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(std::bind(&Application::onWindowClose, this, std::placeholders::_1));
	}

	void Application::run() {
        s_application = this;
        m_running = true;

        const double targetUpdateTime = 1.0 / 60.0;  // Fixed 60 FPS update
        double targetRenderTime = 1.0 / 120.0;  // User-defined render FPS

        auto previousTime = std::chrono::high_resolution_clock::now();
        double accumulatedUpdateTime = 0.0;
        double accumulatedRenderTime = 0.0;

        m_imGuiLayer.onAttach();
        for (auto it : m_layerStack) {
            it->onAttach();
        }
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
                for (auto it : m_layerStack) {
                    it->onUpdate(timestep);
                }
                accumulatedUpdateTime -= targetUpdateTime;
            }

            // Render loop (User-defined FPS)
            if (accumulatedRenderTime >= targetRenderTime) {
                RenderCommand::setClearColor({ 0.2f, 0.2f, 0.3f, 1.0f });
                RenderCommand::clear();

                Render::beginExecution();
                for (auto it : m_layerStack) {
                    it->onRender(Timestep((float)targetRenderTime));
                }
                Render::endExecution();

                ImGuiIO& io = ImGui::GetIO();
                io.DeltaTime = (float)targetRenderTime;
                m_imGuiLayer.begin();
                for (auto it : m_layerStack) {
                    it->onImGUI();
                }
                m_imGuiLayer.end();

                accumulatedRenderTime -= targetRenderTime;

                // Handle window events and update (if necessary)
                m_window->onRender();
            }

            // Optional: Sleep to avoid CPU overuse
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // Detach all layers before shutting down
        for (auto it = m_layerStack.begin(); it != m_layerStack.end(); it++) {
            (*it)->onDetach();
        }
        m_imGuiLayer.onDetach();
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		m_running = false;
		return true;
	}
}
