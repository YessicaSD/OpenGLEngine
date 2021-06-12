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
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale


NAMESPACE_BEGAN
RendererAPI* Renderer::rendererAPI = new OpenGLRendererAPI;


Renderer::Renderer()
{}

Renderer::~Renderer()
{
}

void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{

}

void Renderer::Init()
{
	Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	InitSkybox();
	InitDeferredProgram();
	InitSSAO();
	InitBlurSSAO();

	model = Resources::LoadModel("Assets/backpack/backpack.obj");
	cube = Resources::LoadModel("Assets/cube.fbx");
	renderQuad.reset(Resources::GetQuad());

	material = std::make_unique<Material>();
	material->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/backpack/diffuse.jpg"));
	material->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/backpack/normal.png"));

	std::shared_ptr<Program> program = material->GetProgram();
	program->Bind();
	program->UploadUniformMat4("Model", glm::mat4(1.0));
	program->UploadUniformFloat3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	program->UploadUniformFloat3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	program->UploadUniformFloat3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	program->UploadUniformFloat("material.shininess", 32.0f);

	
}

void Renderer::OnUpdate()
{
	CameraUpdate();
	switch (renderMethod)
	{
		case Shadow::FORWARD:
			ForwardRendering();
			break;
		case Shadow::DEFERRED:
			DeferredRendering();
			break;
	}
}

void Renderer::ForwardRendering()
{

}
void Renderer::DeferredRendering()
{
	GeometryPass();
	SSAOPass();
	LightingPass();
}

void Renderer::GeometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDepthMask(GL_FALSE);

	skyboxHDR->Bind();
	skyProgram->Bind();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	skyProgram->UploadUniformMat4("projViewMatrix", camera.GetProjectionMatrix() * view);
	cube->Draw();
	glDepthMask(GL_TRUE);

	// Draw in the g-buffer ====

	 // activate the texture unit first before binding texture
	material->UseMaterial();
	std::shared_ptr<Program> program = material->GetProgram();
	program->UploadUniformMat4("projViewMatrix", camera.GetProjectViewMatrix());
	program->UploadUniformInt("u_Texture", 0);
	program->UploadUniformMat4("Model", glm::mat4(1.0));
	program->UploadUniformMat4("view", camera.GetViewMatrix());
	program->UploadUniformFloat3("lightPos", lightPos);
	program->UploadUniformFloat3("viewPos", camera.GetPosition());
	model->Draw();
	glm::mat4 tranformation = glm::mat4(1.0);
	tranformation = glm::translate(tranformation, glm::vec3(0.0,-2.0,0.0));
	tranformation = glm::scale(tranformation, glm::vec3(5.0,0.5,5.0));
	program->UploadUniformMat4("Model", tranformation);
	cube->Draw();
}

void Renderer::SSAOPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	//ssaoProgram->UploadUniformInt("gPosition", 0);
	//ssaoProgram->UploadUniformInt("gNormal", 1);
	//ssaoProgram->UploadUniformInt("texNoise", 2);

	gPosition->Bind(0);
	gNormal->Bind(1);
	noiseTex->Bind(2);
	ssaoProgram->Bind();
	renderQuad->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoTex->Bind(0);
	ssaoBlurProgram->Bind();
	renderQuad->Draw();
}

void Renderer::LightingPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Draw in the screen ===
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gPosition->Bind(0);
	gNormal->Bind(1);
	gAlbedoSpec->Bind(2);
	gDepth->Bind(3);
	ssaoTex->Bind(4);
	ssaoBlurTex->Bind(5);

	deferredProgram->Bind();
	deferredProgram->UploadUniformInt("renderMode", renderMode);
	renderQuad->Draw();
}

void Renderer::InitSkybox()
{
	skyProgram.reset(Shadow::LoadProgram("Assets/Programs/skybox.program"));
	skyProgram->UploadUniformInt("skybox", 0);

	skybox = Resources::CreateCubemap();
	skybox->SetPositiveX("Assets/skybox/right.jpg");
	skybox->SetNegativeX("Assets/skybox/left.jpg");
	skybox->SetPositiveY("Assets/skybox/top.jpg");
	skybox->SetNegativeY("Assets/skybox/bottom.jpg");
	skybox->SetPositiveZ("Assets/skybox/front.jpg");
	skybox->SetNegativeZ("Assets/skybox/back.jpg");

	hdrTexture.reset(Resources::LoadTexture("Assets/skybox/kiara_1_dawn_1k.hdr"));
	skyboxHDR.reset(Resources::CreateCubemapFromTexture(hdrTexture.get()));
}

void Renderer::InitDeferredProgram()
{
	deferredProgram.reset(Shadow::LoadProgram("Assets/Programs/deferred.program"));

	deferredProgram->Bind();
	deferredProgram->UploadUniformInt("gPosition", 0);
	deferredProgram->UploadUniformInt("gNormal", 1);
	deferredProgram->UploadUniformInt("gAlbedoSpec", 2);
	deferredProgram->UploadUniformInt("gDepth", 3);
	deferredProgram->UploadUniformInt("gSSAO", 4);
	deferredProgram->UploadUniformInt("gSSAOBlur", 5);

	//Create gBuffer ==
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glEnable(GL_DEPTH_TEST);

	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();

	//glDepthFunc(GL_NOTEQUAL);
	// - position color buffer
	gPosition.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition->GetHandle(), 0);

	// - normal color buffer
	gNormal.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal->GetHandle(), 0);

	// - color + specular color buffer
	gAlbedoSpec.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec->GetHandle(), 0);

	// - depth buffer
	gDepth.reset(Resources::CreateEmptyTexture(w, h, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth->GetHandle(), 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);


	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		SW_LOG_TRACE("Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::GenerateNoiseTexture()
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	noiseTex.reset(Resources::CreateTextureFromArray(ssaoNoise, 4, 4));
}

void Renderer::InitBlurSSAO()
{
	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();

	glGenFramebuffers(1, &ssaoBlurFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	ssaoBlurTex.reset(Resources::CreateEmptyTexture(w, h, GL_RED, GL_RED, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoBlurTex->GetHandle(), 0);

	ssaoBlurProgram.reset(Shadow::LoadProgram("Assets/Programs/blurSSAO.program"));
	ssaoBlurProgram->Bind();
	ssaoBlurProgram->UploadUniformInt("ssaoInput", 0);
}

void Renderer::InitSSAO()
{
	GenerateNoiseTexture();
	kernelsPoint = GenerateKernelPoints(64);

	glGenFramebuffers(1, &ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();

	ssaoTex.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTex->GetHandle(), 0);
	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, attachments);

	ssaoProgram.reset(Shadow::LoadProgram("Assets/Programs/ssao.program"));

	ssaoProgram->Bind();
	ssaoProgram->UploadUniformInt("gPosition",0);
	ssaoProgram->UploadUniformInt("gNormal",1);
	ssaoProgram->UploadUniformInt("texNoise",2);
 	ssaoProgram->UploadUniformFloat3("samples", kernelsPoint);
	ssaoProgram->UploadUniformMat4("projection", camera.GetProjectionMatrix());
}

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

std::vector<glm::vec3> Renderer::GenerateKernelPoints(int number)
{
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	for (unsigned int i = 0; i < number; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	return ssaoKernel;
}

void Renderer::SetViewPort(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
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

	const char* items[] = { "Final", "Normal", "Depth", "Position", "Albedo", "SSAO", "SSAOBlur"};
	ImGui::Combo("Render mode", &renderMode, items, IM_ARRAYSIZE(items));
	ImGui::Text("Light position:");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("#lightx", &lightPos.x);  ImGui::SameLine();
	ImGui::DragFloat("#lighty", &lightPos.y); ImGui::SameLine();
	ImGui::DragFloat("#lightz", &lightPos.z);
	ImGui::PopItemWidth();
	ImGui::End();
}

void Renderer::CameraUpdate()
{
	float speed = 0.5;
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