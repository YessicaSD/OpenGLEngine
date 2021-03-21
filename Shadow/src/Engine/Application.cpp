#include "swpch.h"
#include "Application.h"

#include "Log.h"

#include "glad/glad.h"


NAMESPACE_BEGAN

Application* Application::app = nullptr;
Application::Application()
{
	SW_CORE_ASSERT(app == nullptr, "Application already exists!");
	app = this;
	window = std::unique_ptr<Window>(Window::Create());
}
Application::~Application()
{
}
void Application::Run()
{
	while (running)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		for (auto i = layerManager.begin(); i != layerManager.end(); i++)
		{
			(*i)->OnUpdate();
		}
		window->OnUpdate();
	}
}
void Application::OnEvent(Event& e)
{
	for (auto it = layerManager.end(); it != layerManager.begin(); )
	{
		(*--it)->OnEvent(e);
		//if (e.Handled)
			//break;
	}
}
void Application::PushLayer(Layer* layer)
{
	layerManager.PushLayer(layer);
	layer->OnAttach();
}
void Application::PushOverlay(Layer* layer)
{
	layerManager.PushOverlay(layer);
	layer->OnAttach();
}
NAMESPACE_END
