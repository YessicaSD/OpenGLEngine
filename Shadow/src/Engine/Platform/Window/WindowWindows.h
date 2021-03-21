#pragma once
#include "swpch.h"
#include "Engine/Core.h"
#include "Engine/Window.h"

struct GLFWwindow;

NAMESPACE_BEGAN

	class SHADOW_API WindowWindows : public Window
	{

	public:
		WindowWindows(const WindowProp& props);
		virtual ~WindowWindows();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return data.width; }
		inline unsigned int GetHeight() const override { return data.height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { data.eventCallback = callback; }


		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProp& props);
		virtual void ShutDown();

		GLFWwindow* windowGLFW;

		struct WindowData
		{
			std::string title; 
			unsigned int width, height;
			bool vsync;

			EventCallbackFn eventCallback;
		}
		data;

	};

NAMESPACE_END
