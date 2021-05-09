#include "swpch.h"
#include "Application.h"

#include "Log.h"

#include "glad/glad.h"

#include "Input.h"

#include "glm/glm.hpp"
#include "Shadow/Layers/LayerRenderer.h"
#include "Shadow/Layers/LayerMaterialEditor.h"

NAMESPACE_BEGAN

Application* Application::app = nullptr;

Application::Application()
{
	SW_CORE_ASSERT(app == nullptr, "Application already exists!");
	app = this;
	window = std::unique_ptr<Window>(Window::Create());
	window->SetEventCallback(SW_BIND_FN(Application::OnEvent));

	imguiLayer = new Shadow::LayerImGui();
	PushOverlay(imguiLayer);
	PushOverlay(resourceManager = new Shadow::Resources());
	PushOverlay(new Shadow::Renderer());
	PushOverlay(new Shadow::MaterialEditor());
}
Application::~Application()
{
}
void Application::Run()
{
	for (auto& layer : layerManager)
		layer->Init();

	while (running)
	{
		Renderer::BeginScene();
		Renderer::ClearScreen();

		for (auto& layer : layerManager)
			layer->OnUpdate();

		Renderer::EndScene();


		imguiLayer->Begin();
		
		imguiLayer->StartTopBar();
			for (auto i = layerManager.begin(); i != layerManager.end(); i++)
				(*i)->OnMainTopBar();
		imguiLayer->EndTopBar();

		for (auto i = layerManager.begin(); i != layerManager.end(); i++)
			(*i)->OnImGuiRender();
		imguiLayer->End();

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
