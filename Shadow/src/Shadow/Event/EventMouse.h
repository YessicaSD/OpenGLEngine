#pragma once
#include "Event.h"

namespace Shadow {

	// MOUSE MOVE EVENT ================================================
	class MouseMovedEvent : public Event
	{
		public:
			// resive mouse position
			MouseMovedEvent(const float x, const float y) : mouseX(x), mouseY(y) {}

			float GetX() const { return mouseX; }
			float GetY() const { return mouseY; }

			// return mouse position in string
			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
				return ss.str();
			}
		
			//Define event type
			EVENT_CLASS_TYPE(MOUSE_MOVE)

			//Define category
			EVENT_CLASS_CATEGORY(EVENTCATEGORY_MOUSE | EVENTCATEGORY_INPUT)

		private:
			float mouseX, mouseY;
	};


	// MOUSE SCROLL EVENT ================================================
	class MouseScrolledEvent : public Event
	{
		public:
			MouseScrolledEvent(const float xOffset, const float yOffset)
				: m_XOffset(xOffset), m_YOffset(yOffset) {}

			float GetXOffset() const { return m_XOffset; }
			float GetYOffset() const { return m_YOffset; }

			std::string ToString() const override
			{
				std::stringstream ss;
				ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
				return ss.str();
			}

			EVENT_CLASS_TYPE(MOUSE_SCROLLED)
			EVENT_CLASS_CATEGORY(EVENTCATEGORY_MOUSE | EVENTCATEGORY_INPUT)
		private:
			float m_XOffset, m_YOffset;
	};



	// MOUSE BUTTON EVENT ================================================
	class MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EVENTCATEGORY_MOUSE | EVENTCATEGORY_INPUT | EVENTCATEGORY_MOUSEBUTTON)

		protected:
			MouseButtonEvent(const int button)
				: m_Button(button) {}

			int m_Button;
	};

	// MOUSE BUTTON PRESSED EVENT ================================================
	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESSED)
	};

	// MOUSE BUTTON RELEASED EVENT ================================================
	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASED)
	};

}