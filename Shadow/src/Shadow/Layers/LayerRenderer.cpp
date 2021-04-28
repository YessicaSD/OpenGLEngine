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
#include "Shadow/Application.h"




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

	model = Resources::LoadModel("Assets/gun/model.dae");
	cube = Resources::LoadModel("Assets/cube.fbx");
	material = std::make_unique<Material>();
	std::shared_ptr<Program> program = material->GetProgram();
	program->Bind();
	program->UploadUniformMat4("Model", glm::mat4(1.0));
	program->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	program->UploadUniformFloat3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	program->UploadUniformFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	program->UploadUniformFloat("material.shininess", 32.0f);

	tex = Resources::LoadTexture("Assets/gun/textures/Default_albedo.jpg");
	skybox = Resources::CreateCubemap();
	skybox->SetPositiveX("Assets/skybox/right.jpg");
	skybox->SetNegativeX("Assets/skybox/left.jpg");
	skybox->SetPositiveY("Assets/skybox/top.jpg");
	skybox->SetNegativeY("Assets/skybox/bottom.jpg");
	skybox->SetPositiveZ("Assets/skybox/front.jpg");
	skybox->SetNegativeZ("Assets/skybox/back.jpg");
	
	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();

	//Create gBuffer ==
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// - position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// - normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - color + specular color buffer
	glGenTextures(1, &gColorSpec);
	glBindTexture(GL_TEXTURE_2D, gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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