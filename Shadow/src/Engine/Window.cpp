#include "swpch.h"

#include "Window.h"
#include "Log.h"
#include "Core.h"

#include <GLFW/glfw3.h>
//#include <glad/glad.h>

namespace Shadow {

	std::unique_ptr<Window> Window::Create(WindowProp prop)
	{
		std::unique_ptr<Window> newWindow = std::make_unique<Window>();
		newWindow->data.title = prop.title;
		newWindow->data.width = prop.width;
		newWindow->data.height = prop.height;

		SW_CORE_INFO("Creating window {0} ({1}, {2})", prop.title, prop.width, prop.height);
		int success = glfwInit();
		if (success)
		{
			newWindow->windowGLFW = glfwCreateWindow(prop.width, prop.height, prop.title.c_str(), NULL, NULL);
			glfwMakeContextCurrent(newWindow->windowGLFW);

			// Load glad
			//int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
			//SW_CORE_ASSERT(status, "Failed to initialized Glad!");
			glfwSetWindowUserPointer(newWindow->windowGLFW, &newWindow->data);
			//SetVSync(true);
		}

		return newWindow;
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(windowGLFW);
	}

	void Window::SetVSync(bool enabled)
	{
		glfwSwapInterval((int)enabled);
		data.vsync = enabled;
	}

}
