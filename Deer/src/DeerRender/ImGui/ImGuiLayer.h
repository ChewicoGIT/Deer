#pragma once
#include "DeerRender/Core/Events/ApplicationEvent.h"
#include "DeerRender/Core/Events/KeyEvent.h"
#include "DeerRender/Core/Events/MouseEvent.h"

namespace Deer {
	class ImGuiLayer {
	public:
		~ImGuiLayer() = default;

		void onAttach();
		void onDetach();

		void begin();
		void end();

		void onEvent(Event& event);
	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool onMouseMovedEvent(MouseMovedEvent& e);
		bool onMouseScrollEvent(MouseScrolledEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);
		bool onWindowResizeEvent(WindowResizeEvent& e);
	};
}

