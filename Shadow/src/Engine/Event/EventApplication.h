#pragma once

#include "Event.h"

namespace Shadow {

	class WindowResizedEvent : public Event
	{
	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(APP_TICK)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(APP_UPDATE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(APP_RENDER)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};
}