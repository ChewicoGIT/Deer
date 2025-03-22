#pragma once
#include "DeerRender/Events/Event.h"
#include "Deer/Path.h"

#include <string>
#include <functional>

namespace Deer {
	struct WindowProps {
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProps(const std::string& _title = "Deer Engine",
			unsigned int _width = 1280,
			unsigned int _height = 720)
			: title(_title), width(_width), height(_height)  {
		}
	};

	class Window {
	public:
		virtual ~Window() = default;
		virtual void initWindow() = 0;

		virtual void onRender() = 0;
		virtual void clear() = 0;
		virtual void resolveEvents() = 0;

		inline virtual int getWitdth() const = 0;
		inline virtual int getHeight() const = 0;

		virtual void setEventCallback(std::function<void(Event&)>) = 0;
		virtual void setVSync(bool enabled) = 0;
		inline virtual bool isVSync() const = 0;

		virtual bool getKeyPressed(unsigned int key) = 0;
		virtual bool getMouseButton(int button) = 0;
		virtual void getMousePos(float& x, float& y) = 0;
		virtual void initImGUI() = 0;

		virtual Path folderDialog(const char*) = 0;

		static Window* create(const WindowProps& props = WindowProps());
	};
}