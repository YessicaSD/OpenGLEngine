#include "swpch.h"
#include "Application.h"

#include "Log.h"

#include "glad/glad.h"

#include "Input.h"

#include "glm/glm.hpp"


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
	resourceManager = new Shadow::LayerResourceManager();
	PushOverlay(resourceManager);

}
Application::~Application()
{
}
void Application::Run()
{
	std::string vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	std::string fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	defaultProgram.reset(new Shadow::Program(vertexShaderSource, fragmentShaderSource));

	float vertices[4*3] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[6] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};

	vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
	indexBuffer.reset(IndexBuffer::Create(indices, 6));
	

	BufferLayout layout = {
		{ShaderDataType::FLOAT3, "a_Position"}
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (running)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto layer = layerManager.begin(); layer != layerManager.end(); layer++)
			(*layer)->OnUpdate();

			// ..:: Drawing code (in render loop) :: ..
			// 4. draw the object
			defaultProgram->Bind();

			glBindVertexArray(VAO);
			indexBuffer->Bind();
			glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

		imguiLayer->Begin();
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
