#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Timestep.h"

#ifndef DEER_SERVICE
#include "Deer/Core/Events/Event.h"
#include "Deer/Core/Events/ApplicationEvent.h"
#include "DeerRender/ImGui/ImGuiLayer.h"
#include "DeerRender/Core/Window.h"
#endif

namespace Deer {
	class Application
	{
	public:
		Application(const WindowProps& props = WindowProps());

		static Application* s_application;

		void run();

		virtual void onInit() {}
		virtual void onShutdown() {}
		virtual void onUpdate(Timestep delta) {}

#ifndef DEER_SERVICE
		virtual void onRender(Timestep delta) {}
		virtual void onImGUI() {}
		virtual void onEvent(Event& event) {}

		Scope<Window> m_window;
	private:
		virtual void onEventCallback(Event& e);
		bool onWindowClose(WindowCloseEvent& e);
		ImGuiLayer m_imGuiLayer;
#endif
	private:
		bool m_running;
		float m_lastFrameTime = 0.0f;
	};

}

