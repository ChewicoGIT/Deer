#pragma once
#include "DeerRender/Window.h"
#include "DeerRender/Render/RenderContext.h"
#include <functional>

struct GLFWwindow;

namespace Deer {
	struct LinuxWindowData {
		bool vSync = true;
		int width = 0;
		int height = 0;
		std::function<void(Event&)> callback;
	};

	class LinuxWindow : public Window
	{
	public:
		LinuxWindow(const WindowProps& props);
		virtual ~LinuxWindow();
		void initWindow() override;

		void onRender() override;
		void clear() override;
		void resolveEvents() override;

		inline int getWitdth() const override { return m_data.width; }
		inline int getHeight() const override { return m_data.height; }

		void setEventCallback(std::function<void(Event&)> callback) override { m_data.callback = callback; }
		void setVSync(bool enabled) override;

		bool getKeyPressed(unsigned int key) override;
		bool getMouseButton(int button) override;
		void getMousePos(float& x, float& y) override;
		void initImGUI() override;

		Path folderDialog(const char*) override;

		inline bool isVSync() const override { return m_data.vSync; }
	private:
		GLFWwindow* m_window = nullptr;
		RenderContext* m_renderContext= nullptr;
		LinuxWindowData m_data;
		WindowProps m_windowProps;
	};
}

