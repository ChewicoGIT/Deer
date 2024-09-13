#pragma once
#include "Deer/Core/Window.h"
#include "Deer/Core/Layer.h"
#include "Deer/Core/LayerStack.h"
#include "Deer/Core/Core.h"
#include "Deer/Core/Events/Event.h"
#include "Deer/ImGui/ImGuiLayer.h"

#include "Deer/Core/Events/ApplicationEvent.h"
#include "Deer/Render/Shader.h"
#include "Deer/Render/Buffer.h"
#include "Deer/Render/VertexArray.h"
#include "Deer/Render/Render.h"
#include "Deer/Render/Camera.h"

namespace Deer {
	class Application
	{
	public:
		Application(const WindowProps& props = WindowProps());

		static Application* s_application;

		virtual void onInit() {}
		virtual void onShutdown() {}

		void pushLayer(Layer* layer);
		virtual void onEvent(Event& e);
		void run();

		Scope<Window> m_window;
	private:
		bool onWindowClose(WindowCloseEvent& e);

		bool m_running;
		LayerStack m_layerStack;
		ImGuiLayer m_imGuiLayer;
	private:
		float m_lastFrameTime = 0.0f;
	};

}

