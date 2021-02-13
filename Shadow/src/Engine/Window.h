#pragma once
#include "Event/Event.h"
#include <functional>

NAMESPACE_BEGAN

struct WindowProp
{
	std::string title;
	uint32_t width;
	uint32_t height;
	bool vsync = false;

	WindowProp(const std::string& title = "Shadow Engine",
		uint32_t width = 1600,
		uint32_t height = 900)
		: title(title), width(width), height(height)
	{

	}
};

// This is just an interface
class SHADOW_API Window
{
public:
	using EventCallbackFn = std::function<void(Event&)>;

	virtual ~Window(){}
	virtual void OnUpdate() = 0;

	virtual unsigned int GetWidth() const = 0;
	virtual unsigned int GetHeight() const = 0;

	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool IsVSync() const = 0;


	static Window* Create(const WindowProp& props = WindowProp());

};

NAMESPACE_END