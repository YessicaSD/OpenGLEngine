#pragma once

#include "Event.h"

namespace Shadow {

	class SHADOW_API KeyEvent : public Event
	{
		public:
			int GetKeyCode() const { return keyCode; }

			EVENT_CLASS_CATEGORY(EVENTCATEGORY_KEYBOARD | EVENTCATEGORY_INPUT)

		protected:
			KeyEvent(const int keycode)
				: keyCode(keycode) {}

		int keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const int keycode, const uint16_t repeatCount)
			: KeyEvent(keycode), repeatCount(repeatCount) {}

		uint16_t GetRepeatCount() const { return repeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_PRESSED)
	private:
		uint16_t repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_RELEASED)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(const int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KEY_TYPED)
	};
}