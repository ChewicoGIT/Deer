#pragma once
#include <string>
#include <functional>

namespace Deer {
	enum class EventType
	{
		None = 0,
		WindowClose, WindowMinimize, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseButtonDown, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication =	1 << 0,
		EventCategoryInput =		1 << 1,
		EventCategoryKeyboard =		1 << 2,
		EventCategoryMouse =		1 << 3,
		EventCategoryMouseButton =	1 << 4
	};

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		bool handled = false;

		virtual ~Event() {}
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
		public:
		EventDispatcher(Event& event)
			: m_event(event)
		{
		}
	
		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (m_event.getEventType() == T::getStaticType() && !m_event.handled)
			{
				m_event.handled = func(*(T*)&m_event);
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};
}