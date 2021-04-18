#include "swpch.h"
#include "LayerRenderer.h"
#include <memory>
#include "glad/glad.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Shadow/Input.h"
#include "Shadow/Log.h"
#include "LayerResourceManager.h"

#include <imgui.h>

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

		uniform sampler2D u_Texture;
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

		  vec3 result = (ambient + diffuse) * texture(u_Texture, uv).xyz;
		  FragColor = texture(u_Texture, uv);

		})";

	defaultProgram.reset(Shadow::CreateShader(vertexShaderSource, fragmentShaderSource));

	model = Resources::LoadModel("E:/3D Objects/Patrick/Patrick.obj");
	tex = Resources::LoadTexture("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/Patrick/Flowers.png");
	//tex->Bind();
	//defaultProgram->UploadUniformInt("u_Texture", 0);
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
	float speed = 0.05;
	glm::vec3 cameraPos = camera.GetPosition();
	glm::vec3 cameraRotation = camera.GetRotation();

	std::pair<float, float> mousePos = Input::GetMousePosition();
	glm::vec2 mousePosv = glm::vec2(mousePos.first, mousePos.second);
	static glm::vec2 lastMousePos = mousePosv;

	if (Input::IsMouseButtonPressed(SW_MOUSE_BUTTON_2))
	{
		glm::vec2 offset = mousePosv - lastMousePos;
		cameraRotation.x -= offset.y;
		cameraRotation.y += offset.x;
		
		if (cameraRotation.x > 89.0f)
			cameraRotation.x = 89.0f;
		if (cameraRotation.x < -89.0f)
			cameraRotation.x = -89.0f;

	}
	if (Input::IsKeyPressed(SW_KEY_A))
	{
		cameraPos += camera.GetRight() * speed;
	}
	if (Input::IsKeyPressed(SW_KEY_D))
	{
		cameraPos -= camera.GetRight() * speed;
	}
	if (Input::IsKeyPressed(SW_KEY_W))
	{
		cameraPos += camera.GetForward() * speed;
	}
	if (Input::IsKeyPressed(SW_KEY_S))
	{
		cameraPos -= camera.GetForward() * speed;
	}

	camera.SetRotation(cameraRotation);
	camera.SetPosition(cameraPos);
	
	
	lastMousePos = mousePosv;
	//glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	tex->Bind(1);

	defaultProgram->Bind();
	defaultProgram->UploadUniformMat4("projViewMatrix", camera.GetProjectViewMatrix());
	defaultProgram->UploadUniformInt("u_Texture", 0);
	model->Draw();
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
	vertexArray->Bind();
	rendererAPI->DrawIndexed(vertexArray);
}

void Renderer::OnImGuiRender()
{
	ImGui::Begin("Renderer");
	camera.OnImGuiRender();
	ImGui::End();
}

NAMESPACE_END