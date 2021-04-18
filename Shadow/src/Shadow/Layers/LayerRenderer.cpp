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
		out vec3 vNormal;
		out vec3 FragPos;
		out vec2 uv;

		void main()
		{
			uv = aUV;
			pos = aPos;
		    gl_Position =  projViewMatrix * vec4(aPos, 1.0);
			vNormal = vec3(projViewMatrix * vec4(aNormal,1.0));
			FragPos = aPos;
		})";

	std::string fragmentShaderSource = R"(
		#version 330 core
		out vec4 FragColor;
		in vec3 pos;
		in vec3 vNormal;
		in vec3 FragPos;
		in vec2 uv;

		void main()
		{
		  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
		  vec3 lightPos = vec3(1.0,2.0,2.0);

		  // ambient
		  float ambientStrength = 0.1f;
		  vec3 ambient = ambientStrength * lightColor;

		  // Diffuse 
		  vec3 norm = normalize(vNormal);
		  vec3 lightDir = normalize(lightPos - FragPos);
		  float diff = max(dot(norm, lightDir), 0.0);
		  vec3 diffuse = diff * lightColor;

		  vec3 result = (ambient + diffuse) * vec3(uv,0.0);
		  FragColor = vec4(result, 1.0f);

		})";

	defaultProgram.reset(Shadow::CreateShader(vertexShaderSource, fragmentShaderSource));

	model = Resources::LoadScene("E:/3D Objects/Patrick/Patrick.obj");
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