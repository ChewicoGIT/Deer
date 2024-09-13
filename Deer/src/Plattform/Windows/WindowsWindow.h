#pragma once
#include "Deer/Core/Window.h"
#include "Deer/Render/RenderContext.h"
#include <functional>

struct GLFWwindow;

namespace Deer {
	struct WindowsWindowData {
		bool vSync = true;
		int width = 0;
		int height = 0;
		std::function<void(Event&)> callback;
	};

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void onUpdate() override;
		void clear() override;

		inline int getWitdth() const override { return m_data.width; }
		inline int getHeight() const override { return m_data.height; }

		void setEventCallback(std::function<void(Event&)> callback) override { m_data.callback = callback; }
		void setVSync(bool enabled) override;

		bool getKeyPressed(unsigned int key) override;
		bool getMouseButton(int button) override;
		void getMousePos(float& x, float& y) override;
		void initImGUI() override;

		inline bool isVSync() const override { return m_data.vSync; }
	private:
		GLFWwindow* m_window;
		WindowsWindowData m_data;
		RenderContext* m_renderContext;
	};
}

