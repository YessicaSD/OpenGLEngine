#include "swpch.h"

#include "WindowWindows.h"
#include "Engine/Log.h"
#include  "Engine/Core.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

NAMESPACE_BEGAN

static bool GLFWInitialized = false;

Window* Window::Create(const WindowProp& props)
{
	return new WindowWindows(props);
}

WindowWindows::WindowWindows(const WindowProp& props)
{
	Init(props);
}

WindowWindows::~WindowWindows()
{
	ShutDown();
}


void WindowWindows::Init(const WindowProp& prop)
{
	data.title = prop.title;
	data.width = prop.width;
	data.height = prop.height;

	SW_CORE_INFO("Creating window {0} ({1}, {2})", prop.title, prop.width, prop.height);

	if (!GLFWInitialized)
	{
		int success = glfwInit();
		SW_CORE_ASSERT(success, "Could not intialize GLFW!");
		GLFWInitialized = true;
	}
	
	if (GLFWInitialized)
	{
		windowGLFW = glfwCreateWindow(prop.width, prop.height, prop.title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(windowGLFW);

		// Load glad
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		SW_CORE_ASSERT(status, "Failed to initialized Glad!");
		glfwSetWindowUserPointer(windowGLFW, &data);
		SetVSync(true);

	}
}

void WindowWindows::OnUpdate()
{
	glfwPollEvents();
	glfwSwapBuffers(windowGLFW);
}

void WindowWindows::SetVSync(bool enabled)
{
	glfwSwapInterval((int)enabled);
	data.vsync = enabled;
}

bool WindowWindows::IsVSync() const
{
	return data.vsync;
}

void WindowWindows::ShutDown()
{
	glfwDestroyWindow(windowGLFW);
}



NAMESPACE_END




