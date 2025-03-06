#pragma once
#include "Deer/Core/Core.h"
#include "Deer/Core/Timestep.h"

#ifdef DEER_RENDER
#include "DeerRender/Core/Events/Event.h"
#include "DeerRender/Core/Events/ApplicationEvent.h"
#include "DeerRender/ImGui/ImGuiLayer.h"
#include "DeerRender/Core/Window.h"
#endif

namespace Deer {
	class Application
	{
	public:
		Application();
		
		static Application* s_application;

		int run();

		virtual int onInit() { return 0; }
		virtual int onPreInit() { return 0; }
		virtual void onShutdown() {}
		virtual void onUpdate(Timestep delta) {}

	private:
		bool m_running;
		float m_lastFrameTime = 0.0f;
#ifdef DEER_RENDER
	public:
		Application(const WindowProps& props = WindowProps());

		virtual void onRender(Timestep delta) {}
		virtual void onImGUI() {}
		virtual void onEvent(Event& event) {}

		Scope<Window> m_window;
	private:
		ImGuiLayer m_imGuiLayer;
		const WindowProps m_windowProps;

		virtual void onEventCallback(Event& e);
		void initializeWindow();
		bool onWindowClose(WindowCloseEvent& e);
#endif
	};

}

