#include "swpch.h"

#include "WindowWindows.h"
#include "Shadow/Log.h"
#include "Shadow/Core.h"

#include "Shadow/Event/EventApplication.h"
#include "Shadow/Event/EventMouse.h"
#include "Shadow/Event/EventKey.h"

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

static void GLFWErrorCallback(int error , const char* description)
{
	SW_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
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
		glfwSetErrorCallback(GLFWErrorCallback);
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

		// Set GLFW callbacks
		SetCallbacks();
	}
}

void WindowWindows::SetCallbacks()
{
	glfwSetWindowSizeCallback(windowGLFW, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;
			WindowResizedEvent event(width, height);
			data.eventCallback(event);
		});

	glfwSetWindowCloseCallback(windowGLFW, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});
	glfwSetKeyCallback(windowGLFW, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});

	glfwSetMouseButtonCallback(windowGLFW, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});

	glfwSetScrollCallback(windowGLFW, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});
	glfwSetCursorPosCallback(windowGLFW, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});

	glfwSetCharCallback(windowGLFW, [](GLFWwindow* window, unsigned int character)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(character);
			data.eventCallback(event);
		});
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




