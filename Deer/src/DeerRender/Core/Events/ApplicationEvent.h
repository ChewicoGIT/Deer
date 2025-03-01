#pragma once
#include "DeerRender/Core/Events/Event.h"
#include <sstream>

namespace Deer {
	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int getWidth() const { return m_Width; }
		inline unsigned int getHeight() const { return m_Height; }

		std::string toString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;
		std::string toString() const override {
			return "Window close";
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}