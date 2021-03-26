#include "swpch.h"
#include "Application.h"

#include "Log.h"

#include "glad/glad.h"

#include "Core.h"
#include "Input.h"

NAMESPACE_BEGAN

Application* Application::app = nullptr;

Application::Application()
{
	SW_CORE_ASSERT(app == nullptr, "Application already exists!");
	app = this;
	window = std::unique_ptr<Window>(Window::Create());
	window->SetEventCallback(SW_BIND_FN(Application::OnEvent));

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
			(*i)->OnUpdate();

		//auto [x, y] = Input::GetMousePosition();
		//SW_CORE_TRACE("{0}, {1}", x,y );
		window->OnUpdate();
	}
}

bool Application::OnWindowClosed(WindowCloseEvent& e)
{
	running = false;
	return true;
}

void Application::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(SW_BIND_FN(Application::OnWindowClosed));

	for (auto it = layerManager.end(); it != layerManager.begin(); )
	{
		//SW_INFO(e.ToString());
		
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
