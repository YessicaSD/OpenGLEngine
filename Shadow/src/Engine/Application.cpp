#include "swpch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
NAMESPACE_BEGAN

Application::Application()
{
	window = Window::Create();
}
Application::~Application()
{

}
void Application::Run()
	{
		while (true)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			window->OnUpdate();
		}
	}
NAMESPACE_END
