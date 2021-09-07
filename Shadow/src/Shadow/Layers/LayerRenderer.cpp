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
#include <glm/gtc/matrix_inverse.hpp>

NAMESPACE_BEGAN
RendererAPI* Renderer::rendererAPI = new OpenGLRendererAPI;
Renderer* Renderer::instance = new Renderer;

Renderer::Renderer()
{}


Renderer::~Renderer()
{
	for each (std::shared_ptr<Environment> var in environments)
	{
		var.reset();
	}
	environments.clear();
	materialGun.reset();
	gAlbedoSpec.reset();
	gNormal.reset();
	gData.reset();
	gFBO.reset();

	SW_CORE_INFO("Deleting renderer");
}

void Renderer::BeginScene()
{
}

void Renderer::EndScene()
{

}

void Renderer::Init()
{
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });

	cube.reset(Resources::LoadModel("Assets/cube.fbx"));
	renderQuad.reset(Resources::GetQuad());

	InitSkybox();
	InitBrdf();
	InitDeferredProgram();
	InitSSAO();
	InitBlurSSAO();
	InitHdrFBO();
	InitGeometrypass();



	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

}

#pragma region Init

void Renderer::InitSkybox()
{
	skyProgram.reset(Shadow::LoadProgram("Assets/Programs/skybox.glsl"));
	skyProgram->UploadUniformInt("skybox", 0);

	std::shared_ptr<Texture> tex(Resources::LoadTexture("Assets/skybox/03-Ueno-Shrine_3k.hdr"));
	std::shared_ptr<Cubemap> cube(Resources::CreateCubemapFromTexture(tex.get()));
	std::shared_ptr<Environment> environment(new Environment());
	environment->SetSkybox(cube);
	environments.push_back(environment);

	std::shared_ptr<Texture> tex2(Resources::LoadTexture("Assets/skybox/Arches_E_PineTree_3k.hdr"));
	std::shared_ptr<Cubemap> cube2(Resources::CreateCubemapFromTexture(tex2.get()));
	std::shared_ptr<Environment> environment2(new Environment());
	environment2->SetSkybox(cube2);
	environments.push_back(environment2);


	std::shared_ptr<Texture> tex3(Resources::LoadTexture("Assets/skybox/reinforced_concrete_02_1k.hdr"));
	std::shared_ptr<Cubemap> cube3(Resources::CreateCubemapFromTexture(tex3.get()));
	std::shared_ptr<Environment> environment3(new Environment());
	environment3->SetSkybox(cube3);
	environments.push_back(environment3);

	hdrTexture.reset(Resources::LoadTexture("Assets/skybox/kiara_1_dawn_1k.hdr"));
	skyboxHDR.reset(Resources::CreateCubemapFromTexture(hdrTexture.get()));
	std::shared_ptr<Environment> environment4(new Environment());
	environment4->SetSkybox(skyboxHDR);
	environments.push_back(environment4);

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

void Renderer::InitBrdf()
{
	brdfProgram.reset(Shadow::LoadProgram("Assets/Programs/brdf.glsl"));

	Renderer::SetViewPort(0, 0, 512, 512);

	//brdfFBO.reset(Resources::CreateFBO());
	//brdfFBO->Bind();

	Resources::instance->bakeFBO->Bind();
	Resources::instance->bakeRBO->Bind();
	Resources::instance->bakeRBO->BindDepthToFrameBuffer();
	Resources::instance->bakeRBO->DefineDepthStorageSize(512);


	Window& window = Application::Get().GetWindow();
	float w = window.GetWidth();
	float h = window.GetHeight();

	brdfLutTexture.reset(Resources::CreateEmptyTexture(512, 512, GL_RG16F, GL_RG, GL_FLOAT, false));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLutTexture->GetHandle(), 0);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0, 0, 512, 512);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	brdfProgram->Bind();
	renderQuad->Draw();

	Renderer::SetViewPort(0, 0, w, h);
}

void Renderer::InitHdrFBO()
{
	Window& window = Application::Get().GetWindow();
	float w = window.GetWidth();
	float h = window.GetHeight();

	hdrFBO.reset(Resources::CreateFBO());
	hdrFBO->Bind();

	renderTex.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex->GetHandle(), 0);

	highlightsTex.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// attach texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, highlightsTex->GetHandle(), 0);

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	blurBloomProgram.reset(LoadProgram("Assets/Programs/BlurBloom.glsl"));
	blurBloomProgram->Bind();
	blurBloomProgram->UploadUniformInt("image", 0);
	for (unsigned int i = 0; i < 2; i++)
	{
		pingpongFBO[i] = Resources::CreateFBO();
		pingpongFBO[i]->Bind();
		pingpongBuffer[i] = Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i]->GetHandle(), 0
		);
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);
	}

	finalBloom.reset(LoadProgram("Assets/Programs/FinalBloom.glsl"));
	finalBloom->Bind();
	finalBloom->UploadUniformInt("scene", 0);
	finalBloom->UploadUniformInt("bloomBlur", 1);
}

void Renderer::InitGeometrypass()
{
	geometryPassProgram.reset(LoadProgram("Assets/Programs/GeometryPass.glsl"));
	geometryPassProgram->Bind();
	geometryPassProgram->UploadUniformInt("albedoTex", 0);
	geometryPassProgram->UploadUniformInt("normalTex", 1);
	geometryPassProgram->UploadUniformInt("roughnessTex", 2);
	geometryPassProgram->UploadUniformInt("metalTex", 3);
	geometryPassProgram->UploadUniformMat4("Model", glm::mat4(1.0));
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
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);

	ssaoProgram.reset(Shadow::LoadProgram("Assets/Programs/ssao.glsl"));

	ssaoProgram->Bind();
	ssaoProgram->UploadUniformInt("gPosition", 0);
	ssaoProgram->UploadUniformInt("gNormal", 1);
	ssaoProgram->UploadUniformInt("texNoise", 2);
	ssaoProgram->UploadUniformFloat3("samples", kernelsPoint);
	ssaoProgram->UploadUniformMat4("projection", camera.GetProjectionMatrix());
}

void Renderer::InitDeferredProgram()
{
	deferredProgram.reset(Shadow::LoadProgram("Assets/Programs/deferred.glsl"));

	deferredProgram->Bind();
	deferredProgram->UploadUniformInt("gPosition", 0);
	deferredProgram->UploadUniformInt("gNormal", 1);
	deferredProgram->UploadUniformInt("gAlbedoSpec", 2);
	deferredProgram->UploadUniformInt("gDepth", 3);
	deferredProgram->UploadUniformInt("gSSAO", 4);
	deferredProgram->UploadUniformInt("gSSAOBlur", 5);
	deferredProgram->UploadUniformInt("gData", 6);
	deferredProgram->UploadUniformInt("brdfLUT", 7);
	deferredProgram->UploadUniformInt("irradianceMap", 8);
	deferredProgram->UploadUniformInt("prefilterMap", 9);


	//Create gBuffer ==
	gFBO.reset(Resources::CreateFBO());
	gFBO->Bind();

	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();

	
	// - position color buffer
	gPosition.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGB, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition->GetHandle(), 0);

	// - normal color buffer
	gNormal.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA16F, GL_RGB, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal->GetHandle(), 0);

	// - color + specular color buffer
	gAlbedoSpec.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec->GetHandle(), 0);

	// roughness color buffer
	gData.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA, GL_RGB, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gData->GetHandle(), 0);


	// - depth buffer
	gDepth.reset(Resources::CreateEmptyTexture(w, h, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth->GetHandle(), 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	const int size = 5;
	unsigned int attachments[size] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(size, attachments);


	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		SW_LOG_TRACE("Framebuffer not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	FBO_FinalRender.reset(Resources::CreateFBO());
	RBO_FinalRender.reset(Resources::CreateRBO());
	FBO_FinalRender->Bind();

	finalRender.reset(Resources::CreateEmptyTexture(w, h, GL_RGBA, GL_RGBA, GL_FLOAT, false));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalRender->GetHandle(), 0);
	
}
#pragma endregion

#pragma region rending

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

void Renderer::SSAOPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT);

	gPosition->Bind(0);
	gNormal->Bind(1);
	noiseTex->Bind(2);
	ssaoProgram->Bind();
	ssaoProgram->UploadUniformInt("rangeCheckActive", ssaoRangeCheck);
	ssaoProgram->UploadUniformFloat("power", ssaoIntesity);
	ssaoProgram->UploadUniformMat4("view", camera.GetViewMatrix());
	renderQuad->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	ssaoTex->Bind(0);
	ssaoBlurProgram->Bind();
	
	renderQuad->Draw();
}

void Renderer::ForwardRendering()
{

}
void Renderer::DeferredRendering()
{
	
	GeometryPass();
	SSAOPass();
	LightingPass();
	BloomPass();
}

void Renderer::GeometryPass()
{
	gFBO->Bind();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	EnvironmentMap();

	// Draw in the g-buffer ====
	geometryPassProgram->Bind();
	geometryPassProgram->UploadUniformMat4("projViewMatrix", camera.GetProjectViewMatrix());
	geometryPassProgram->UploadUniformMat4("view", camera.GetViewMatrix());
	geometryPassProgram->UploadUniformFloat3("viewPos", camera.GetPosition());

	for each (Entity var in instance->entities)
	{
		if (!var.enabled)
			continue;

		std::shared_ptr<Material> currMat = var.GetMaterial();
		currMat->UseMaterial();
		geometryPassProgram->UploadUniformBoolArray("activeTextures", currMat->GetActiveTextures(), TextureType::MAX_TEXTURE);
		geometryPassProgram->UploadUniformFloat3("color", currMat->GetColor());
		geometryPassProgram->UploadUniformFloat2("rmValue", currMat->GetRoughnessMetalness());
		geometryPassProgram->UploadUniformMat4("Model", var.GetModel());
		var.Draw();
	}
}

void Renderer::EnvironmentMap()
{
	gFBO->Bind();
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	std::shared_ptr<Environment> currEnvironment = environments[currentEnvironment];
	switch (skyboxIndex)
	{
		case 0: currEnvironment->GetSkybox()->Bind();  break;
		case 1: currEnvironment->GetIrradiance()->Bind(); break;
		case 2: currEnvironment->GetPrefilter()->Bind(); break;
	}

	skyProgram->Bind();
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	skyProgram->UploadUniformMat4("projViewMatrix", camera.GetProjectionMatrix() * view);
	cube->Draw();
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}

void Renderer::LightingPass()
{
	hdrFBO->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gPosition->Bind(0);
	gNormal->Bind(1);
	gAlbedoSpec->Bind(2);
	gDepth->Bind(3);
	ssaoTex->Bind(4);
	ssaoBlurTex->Bind(5);
	gData->Bind(6);
	brdfLutTexture->Bind(7);
	std::shared_ptr<Environment> currEnvironment = environments[currentEnvironment];
	//environment->GetBRDF()->Bind(7);
	currEnvironment->GetIrradiance()->Bind(8);
	currEnvironment->GetPrefilter()->Bind(9);

	deferredProgram->Bind();
	deferredProgram->UploadUniformInt("renderMode", renderMode);
	deferredProgram->UploadUniformFloat3("camPos", camera.GetPosition());
	deferredProgram->UploadUniformInt("activeSSAO", SSAO);
	deferredProgram->UploadUniformFloat("bloomThreshold", bloomThreshold);
	deferredProgram->UploadUniformFloat("ssaoIntesity", 1);
	SendLights(deferredProgram);
	renderQuad->Draw();
}


void Renderer::BloomPass()
{
	bool horizontal = true, first_iteration = true;
	blurBloomProgram->Bind();
	for (unsigned int i = 0; i < bloomBlurRange; i++)
	{
		pingpongFBO[horizontal]->Bind();
		blurBloomProgram->UploadUniformInt("horizontal", horizontal);
		first_iteration ? highlightsTex->Bind(0) : pingpongBuffer[!horizontal]->Bind(0);
		renderQuad->Draw();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	
	FBO_FinalRender->Bind();
	RBO_FinalRender->Bind();
	RBO_FinalRender->BindDepthToFrameBuffer();
	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();

	RBO_FinalRender->DefineDepthStorageSize(w, h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalRender->GetHandle(), 0);

	finalBloom->Bind();

	renderTex->Bind(0);
	if (bloomBlurRange != 0)
	{
		pingpongBuffer[!horizontal]->Bind(1);
	}
	else
	{
		highlightsTex->Bind(1);
	}

	finalBloom->UploadUniformFloat("exposure", bloomRange);
	finalBloom->UploadUniformInt("mode", finalMode);

	renderQuad->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}
#pragma endregion

void Renderer::SendLights(std::shared_ptr<Program> program)
{
	program->UploadUniformInt("numLights", instance->lights.size());
	for (int i = 0; i < instance->lights.size(); i++)
	{
		Light* clight = instance->lights[i];
		program->UploadUniformInt("lights[" + std::to_string(i) + "].type", (int)clight->type);
		program->UploadUniformFloat3("lights[" + std::to_string(i) + "].position", clight->position);
		program->UploadUniformFloat3("lights[" + std::to_string(i) + "].color", clight->color);

		if (clight->type == LightTypes::DIRECTIONAL)
		{
			DirectionalLight* dLight = (DirectionalLight*)clight;
			program->UploadUniformFloat3("lights[" + std::to_string(i) + "].direction", dLight->direction);
		}
	}
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

void Renderer::EntitiesUI()
{
	ImGui::Begin("Entities");
	for (int i=0; i< instance->entities.size(); i++)
	{
		Entity& var = instance->entities[i];
		std::string name = "Entity" + std::to_string(i);
		if (var.name != "")
			name = var.name;

		if (ImGui::CollapsingHeader(name.c_str()))
		{
			name = "##enabled" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::Checkbox("Enabled", &var.enabled);
			ImGui::PopID();

			name = "##position" + std::to_string(i);
			glm::vec3 aux = var.GetPosition();
			ImGui::DragFloat3(name.c_str(), &aux.x, 0.1);
			var.SetPosition(aux);

			name = "##rotation" + std::to_string(i);
			aux = var.GetRotation();
			ImGui::DragFloat3(name.c_str(), &aux.x,0.1);
			var.SetRotation(aux);

			name = "##scale" + std::to_string(i);
			aux = var.GetScale();
			ImGui::DragFloat3(name.c_str(), &aux.x, 0.1);
			var.SetScale(aux);

			std::shared_ptr<Material> currMaterial = var.GetMaterial();
			bool* activeTex = currMaterial->GetActiveTextures();
			name = "##colorText" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::Checkbox("Active Color texture ", &activeTex[0]);
			ImGui::PopID();

			name = "##colorPicker" + std::to_string(i);
			ImGui::PushID(name.c_str());
			float color[3] = { currMaterial->GetColor().x, currMaterial->GetColor().y, currMaterial->GetColor().z };
			ImGui::ColorPicker3("Color", color);
			currMaterial->SetColor(color);
			ImGui::PopID();

			name = "##roughnessTex" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::Checkbox("Active Roughness Texture", &activeTex[2]);
			ImGui::PopID();
			
			name = "##roughness" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::DragFloat("Roughness", &currMaterial->GetRoughnessMetalness().x, 0.1, 0.0, 1.0);
			ImGui::PopID();

			name = "##MetalnessTex" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::Checkbox("Active Metal Texture", &activeTex[3]);
			ImGui::PopID();
			
			name = "##Metalness" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::DragFloat("Metalness", &currMaterial->GetRoughnessMetalness().y, 0.1, 0.0, 1.0);
			ImGui::PopID();

			name = "##NormalTex" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::Checkbox("Active Normal Texture", &activeTex[1]);
			ImGui::PopID();

			name = "##AoTex" + std::to_string(i);
			ImGui::PushID(name.c_str());
			ImGui::Checkbox("Active AO Texture", &activeTex[TextureType::AO]);
			ImGui::PopID();
		}

	}
	ImGui::End();
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
	ImGui::Begin("Scene");
	ImVec2 wsize = ImGui::GetWindowSize();
	float w = Application::Get().GetWindow().GetWidth();
	float h = Application::Get().GetWindow().GetHeight();
	ImVec2 windowSize = ImVec2(w, h);

	//ImGui::GetWindowDrawList()->AddImage(
	//	(void*)(intptr_t)finalRender->GetHandle(), ImVec2(ImGui::GetItemRectMin().x,
	//		ImGui::GetItemRectMin().y),
	//	ImVec2(finalRender->GetWidth(), finalRender->GetHeight()), ImVec2(0, 1), ImVec2(1, 0));
	float height = (ImGui::GetWindowWidth() / finalRender->GetWidth()) * finalRender->GetHeight();
	float heightCursor = ImGui::GetWindowHeight() * 0.5f - height * 0.5f;
	if (heightCursor < 0) heightCursor = 0;
	ImGui::SetCursorPosY(heightCursor);
	ImGui::Image((void*)(intptr_t)finalRender->GetHandle(), ImVec2(ImGui::GetWindowWidth(), height), ImVec2(0, 1), ImVec2(1, 0));

	
	
	ImGui::End();
	ImGui::Begin("Renderer");
	camera.OnImGuiRender();

	const char* items[] = { "Final", "Normal", "Depth", "Position", "Albedo", "SSAO", "SSAOBlur", "Roughness", "Metal", "AO","brdf", "irradiance", "prefilterMap", "All At Ones"};
	ImGui::Combo("Render mode", &renderMode, items, IM_ARRAYSIZE(items));

	const char* items2[] = { "Final", "Scene", "HightLight" };
	ImGui::Combo("Render mode Bloom", &finalMode, items2, IM_ARRAYSIZE(items2));

	const char* items4[] = { "Shrine", "PineTree", "Christmas", "Dessert" };
	ImGui::Combo("Change skybox", &currentEnvironment, items4, IM_ARRAYSIZE(items4));

	const char* items3[] = { "Skybox", "Irradiance", "Prefilter" };
	ImGui::Combo("Skybox type", &skyboxIndex, items3, IM_ARRAYSIZE(items3));

	

	ImGui::SliderFloat("Expesure range", &bloomRange, 0, 50.0);

	ImGui::SliderInt("Bloom Blur range", &bloomBlurRange, 0, 100);

	ImGui::SliderFloat("Bloom threshold", &bloomThreshold, 0.0,1.0);

	if (ImGui::CollapsingHeader("SSAO"))
	{
		ImGui::Checkbox("Active SSAO", &SSAO);
		ImGui::Checkbox("Range check", &ssaoRangeCheck);
		ImGui::SliderFloat("SSAO intensity", &ssaoIntesity, 0.0,10.0);
	}

	ImGui::Text("Light position:");
	ImGui::BeginGroup();
	ImGui::PushItemWidth(100);
	int i = 0;
	for each (Light* light in instance->lights)
	{
		ImGui::Text("Position:");
		std::string label = "##lightx"; label += std::to_string(i);
		ImGui::DragFloat(label.c_str(), &light->position.x);  ImGui::SameLine();
		label = "##lighty"; label += std::to_string(i);
		ImGui::DragFloat(label.c_str(), &light->position.y); ImGui::SameLine();
		label = "##lightz"; label += std::to_string(i);
		ImGui::DragFloat(label.c_str(), &light->position.z);

		ImGui::Text("Color:");
		label = "##colorr"; label += std::to_string(i);
		ImGui::ColorPicker3(label.c_str(), &light->color.x);
		i++;
	}


	ImGui::PopItemWidth();
	ImGui::EndGroup();
	
	
	ImGui::End();

	EntitiesUI();
}

void Renderer::PushEntity(Entity entity)
{
	instance->entities.push_back(entity);
}

void Renderer::PushLight(Light* light)
{
	instance->lights.push_back(light);
}

void Renderer::CameraUpdate()
{
	camera.CameraUpdatedInput();
}

NAMESPACE_END