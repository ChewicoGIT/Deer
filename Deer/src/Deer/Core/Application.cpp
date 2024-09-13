#include "Application.h"
#include "Deer/Core/Log.h"
#include "Deer/Core/Timestep.h"

#include "Deer/Render/RenderCommand.h"

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

		m_imGuiLayer.onAttach();
		for (auto it : m_layerStack) {
			it->onAttach();
		}
		onInit();

		while (m_running) {
			// Temp, implement time implementation per plattform
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			RenderCommand::setClearColor({ 0.2f, 0.2f, 0.3f, 1.0f });
			RenderCommand::clear();

			Render::beginScene();
			for (auto it : m_layerStack) {
				it->onUpdate(timestep);
			}
			Render::endScene();

			ImGuiIO& io = ImGui::GetIO();
			io.DeltaTime = timestep.getSeconds();
			m_imGuiLayer.begin();
			for (auto it : m_layerStack) {
				it->onImGUI();
			}
			m_imGuiLayer.end();

			m_window->onUpdate();
		}

		//Deatach all layers before shuting down
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
