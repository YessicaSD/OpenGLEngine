#pragma once
#include "Shadow/Core.h"

#include <string>
#include <functional>
#include <sstream>

namespace Shadow
{
	enum class EventType
	{
		NONE  = 0, 
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WNDOW_LOSTFOCUS, WINDOW_MOVE,
		APP_TICK, APP_UPDATE, APP_RENDER,
		KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
		MOUSE_BUTTON_PRESSED, MOUSE_BUTTON_RELEASED, MOUSE_MOVE, MOUSE_SCROLLED,
	};

	// BTI MASK
	enum EventCategory
	{
		NONE = 0,
		EVENTCATEGORY_APPLICATION	= BIT(0),
		EVENTCATEGORY_INPUT			= BIT(1),
		EVENTCATEGORY_KEYBOARD		= BIT(2),
		EVENTCATEGORY_MOUSE			= BIT(3),
		EVENTCATEGORY_MOUSEBUTTON	= BIT(4),
	};

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class SHADOW_API Event
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }
		
		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	protected:
		bool handled = false;

	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool (T&)>;

	public:
		EventDispatcher(Event& event) : myEvent(event) {}

		// F will be deduced by the compiler
		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			if (myEvent.GetEventType() == T::GetStaticType())
			{
				myEvent.handled |= func(*(T*)&myEvent);
				return true;
			}
			return false;
		}
	private:
		Event& myEvent;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
