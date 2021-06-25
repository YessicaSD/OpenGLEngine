#include "swpch.h"
#include "Environment.h"
#include "Shadow/Layers/LayerResourceManager.h"
#include "Shadow/Layers/LayerRenderer.h"
#include "Shadow/Application.h"
#include "Shadow/Window.h"

#include "glad/glad.h"

NAMESPACE_BEGAN

Environment::~Environment()
{
	skybox.reset();
	irradiance.reset();
}

void Environment::SetSkybox(std::shared_ptr<Cubemap> skybox)
{
	this->skybox = skybox;

	// Create Irradiance ==================
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

	irradiance.reset(Resources::CreateCubemap());
	irradiance->SetTexturesSize(32, 32);

	Resources::instance->bakeFBO->Bind();
	Resources::instance->bakeRBO->Bind();
	Resources::instance->bakeRBO->DefineDepthStorageSize(32);
	Resources::instance->bakeRBO->BindDepthToFrameBuffer();
	

	Resources::instance->irradianceProgram->Bind();
	Resources::instance->irradianceProgram->UploadUniformInt("environment",0);
	Resources::instance->irradianceProgram->UploadUniformMat4("projection", captureProjection);
	skybox->Bind(0);

	Renderer::SetViewPort(0, 0, 32, 32);

	for (unsigned int i = 0; i < 6; ++i)
	{
		Resources::instance->irradianceProgram->UploadUniformMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance->GetHandle(), 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Resources::instance->cubeModel->Draw();
	}

	// Create pre-filter map ===========================

	prefilter.reset(Resources::CreateCubemap());
	prefilter->SetTexturesSize(128, 128);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
	prefilter->GenerateMipmap();
	Resources::instance->prefilterProgram->Bind();
	this->skybox->Bind();
	Resources::instance->prefilterProgram->UploadUniformMat4("projection", captureProjection);
	unsigned int maxMipLevels = 5;
	for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
	{
		// reisze framebuffer according to mip-level size.
		unsigned int mipWidth = 128 * std::pow(0.5, mip);
		unsigned int mipHeight = 128 * std::pow(0.5, mip);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		Renderer::SetViewPort(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevels - 1);
		Resources::instance->prefilterProgram->UploadUniformFloat("roughness", roughness);
		for (unsigned int i = 0; i < 6; ++i)
		{
			Resources::instance->prefilterProgram->UploadUniformMat4("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter->GetHandle(), mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			Resources::instance->cubeModel->Draw();
		}
	}

	/*brdfTex.reset(Resources::CreateEmptyTexture(512, 512, GL_RG16F, GL_RG, GL_FLOAT));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfTex->GetHandle(), 0);
	Resources::instance->bakeRBO->DefineDepthStorageSize(512);

	glViewport(0, 0, 512, 512);
	Resources::instance->brdfProgram->Bind();
	Resources::instance->quadModel->Draw();*/

	Window& window = Application::Get().GetWindow();
	Renderer::SetViewPort(0, 0, window.GetWidth(), window.GetHeight());

}
NAMESPACE_END

