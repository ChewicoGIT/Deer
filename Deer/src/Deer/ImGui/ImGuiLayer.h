#pragma once
#include "Deer/Core/Layer.h"
#include "Deer/Core/Events/ApplicationEvent.h"
#include "Deer/Core/Events/KeyEvent.h"
#include "Deer/Core/Events/MouseEvent.h"
#include "imgui.h"

namespace Deer {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer()
			: Layer("ImGuiLayer") {
		}

		~ImGuiLayer() = default;

		void onAttach() override;
		void onDetach() override;

		void begin();
		void end();

		void onEvent(Event& event) override;
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

