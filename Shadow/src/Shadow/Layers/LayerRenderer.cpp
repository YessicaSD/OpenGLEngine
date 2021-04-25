#include "swpch.h"
#include "LayerRenderer.h"
#include <memory>
#include "glad/glad.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Shadow/Input.h"
#include "Shadow/Log.h"
#include "LayerResourceManager.h"

#include <imgui.h>
#include <imgui_node_editor.h>




NAMESPACE_BEGAN
RendererAPI* Renderer::rendererAPI = new OpenGLRendererAPI;


Renderer::Renderer()
{


	Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	
	std::string vsSky = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;

		out vec3 TexCoords;

		uniform mat4 projViewMatrix;
		

		void main()
		{
			TexCoords = aPos;
			gl_Position = projViewMatrix * vec4(aPos, 1.0);
		})";

	std::string fsSky = R"(
		#version 330 core
		out vec4 FragColor;

		in vec3 TexCoords;

		uniform samplerCube skybox;

		void main()
		{    
			FragColor = texture(skybox, TexCoords);
		})";

	skyProgram.reset(Shadow::CreateShader(vsSky, fsSky));
	skyProgram->UploadUniformInt("skybox", 0);

	model = Resources::LoadModel("E:/3D Objects/Patrick/Patrick.obj");
	cube = Resources::LoadModel("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/cube.fbx");
	material = std::make_unique<Material>();
	std::shared_ptr<Program> program = material->GetProgram();
	program->Bind();
	program->UploadUniformMat4("Model", glm::mat4(1.0));
	program->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	program->UploadUniformFloat3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	program->UploadUniformFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	program->UploadUniformFloat("material.shininess", 32.0f);

	tex = Resources::LoadTexture("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/Patrick/Flowers.png");
	skybox = Resources::CreateCubemap();
	skybox->SetPositiveX("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/skybox/right.jpg");
	skybox->SetNegativeX("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/skybox/left.jpg");
	skybox->SetPositiveY("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/skybox/top.jpg");
	skybox->SetNegativeY("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/skybox/bottom.jpg");
	skybox->SetPositiveZ("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/skybox/front.jpg");
	skybox->SetNegativeZ("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/skybox/back.jpg");
	
	glEnable(GL_DEPTH_TEST);
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
	CameraUpdate();

	glDepthMask(GL_FALSE);

	skybox->Bind();
	skyProgram->Bind();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	skyProgram->UploadUniformMat4("projViewMatrix", camera.GetProjectionMatrix() * view);
	cube->Draw();

	glDepthMask(GL_TRUE);
	//glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	tex->Bind(1);
	material->UseMaterial();

	std::shared_ptr<Program> program = material->GetProgram();
	program->UploadUniformMat4("projViewMatrix", camera.GetProjectViewMatrix());
	program->UploadUniformInt("u_Texture", 0);
	program->UploadUniformMat4("Model", glm::mat4(1.0));

	model->Draw();
}

void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
{
	vertexArray->Bind();
	rendererAPI->DrawIndexed(vertexArray);
}


void Renderer::OnImGuiRender()
{
	ImGui::Begin("Scene Info");
	std::vector<Mesh> meshes = model->GetMeshes();
	if (ImGui::TreeNode("Model"))
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			if (ImGui::TreeNode((void*)(intptr_t)i, meshes[i].GetName().c_str()))
			{
				//ImGui::Text(meshes[i].GetName().c_str());
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();

	ImGui::Begin("Renderer");
	camera.OnImGuiRender();
	ImGui::End();
}

void Renderer::CameraUpdate()
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
}

NAMESPACE_END