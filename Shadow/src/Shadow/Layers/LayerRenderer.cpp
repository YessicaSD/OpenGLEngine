#include "swpch.h"
#include "LayerRenderer.h"
#include <memory>
#include "glad/glad.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Shadow/Input.h"
#include "Shadow/Log.h"
#include "LayerResourceManager.h"
NAMESPACE_BEGAN

RendererAPI* Renderer::rendererAPI = new OpenGLRendererAPI;

Renderer::Renderer()
{
	Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	glEnable(GL_DEPTH_TEST);

	std::string vertexShaderSource = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aUV;

		uniform mat4 projViewMatrix;

		out vec3 pos;

		void main()
		{
			pos = aPos;
		   gl_Position =  projViewMatrix * vec4(aPos, 1.0);
		})";

	std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec3 pos;

		void main()
		{
		   FragColor = vec4(pos, 1.0f);
		})";

	defaultProgram.reset(new Shadow::Program(vertexShaderSource, fragmentShaderSource));

	model = Resources::LoadScene("E:/3D Objects/cube.fbx");
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
	glm::vec3 cameraPos = camera.GetPosition();
	if (Input::IsKeyPressed(SW_KEY_LEFT))
	{
		cameraPos += camera.GetRight();
	}
	if (Input::IsKeyPressed(SW_KEY_RIGHT))
	{
		cameraPos -= camera.GetRight();
	}
	if (Input::IsKeyPressed(SW_KEY_UP))
	{
		cameraPos -= camera.GetUp();
	}
	if (Input::IsKeyPressed(SW_KEY_DOWN))
	{
		cameraPos += camera.GetUp();
	}
	camera.SetPosition(cameraPos);

	defaultProgram->Bind();
	defaultProgram->UploadUniformMat4("projViewMatrix", camera.GetProjectViewMatrix());

	model->Draw();

}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
	vertexArray->Bind();
	rendererAPI->DrawIndexed(vertexArray);
}

NAMESPACE_END