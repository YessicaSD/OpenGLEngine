#pragma once
#include "swpch.h"
#include "Core.h"

struct GLFWwindow;

namespace Shadow {

	class SHADOW_API Window
	{
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

	public:
		Window() {};
		static std::unique_ptr<Window> Create(WindowProp prop = WindowProp());
		void OnUpdate();
		void SetVSync(bool enabled);

		virtual ~Window() {}
	private:
		WindowProp data;
		GLFWwindow* windowGLFW;

	};

}
