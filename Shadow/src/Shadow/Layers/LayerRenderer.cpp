#include "swpch.h"
#include "LayerRenderer.h"
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "glad/glad.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Shadow/Input.h"
#include "Shadow/Log.h"
NAMESPACE_BEGAN

RendererAPI* Renderer::rendererAPI = new OpenGLRendererAPI;

Renderer::Renderer()
{
	Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		uniform mat4 projViewMatrix;
		void main()
		{
		   gl_Position =  projViewMatrix * vec4(aPos, 1.0);
		})";

	std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		void main()
		{
		   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
		})";

	defaultProgram.reset(new Shadow::Program(vertexShaderSource, fragmentShaderSource));

	float vertices[4 * 3] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[6] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
	};
	std::shared_ptr<VertexBuffer> vertexBuffer;
	vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
	std::shared_ptr<IndexBuffer> indexBuffer;
	indexBuffer.reset(IndexBuffer::Create(indices, 6));
	vertexArray.reset(VertexArray::Create());
	vertexBuffer->SetLayout({
		{ShaderDataType::FLOAT3, "a_Position"}
		});

	vertexArray->AddVertexBuffer(vertexBuffer);
	vertexArray->SetIndexBuffer(indexBuffer);
	glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::translate(trans, glm::vec3(0.10f, -0.10f, 0.0f));
	defaultProgram->Bind();
	projViewUniform = glGetUniformLocation(defaultProgram->GetProgramID(), "projViewMatrix");
	glUniformMatrix4fv(projViewUniform, 1, GL_FALSE, glm::value_ptr(camera.GetProjectViewMatrix()));
}

Renderer::~Renderer()
{
}

void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{
}

void Renderer::OnUpdate()
{
	if (Input::IsKeyPressed(SW_KEY_LEFT))
	{
		SW_INFO("left arrow is pressed");
	}
	defaultProgram->Bind();
	vertexArray->Bind();
	
	Renderer::Submit(vertexArray);
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
	vertexArray->Bind();
	rendererAPI->DrawIndexed(vertexArray);
}

NAMESPACE_END