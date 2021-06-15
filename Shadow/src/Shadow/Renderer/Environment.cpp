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

	Window& window = Application::Get().GetWindow();
	Renderer::SetViewPort(0, 0, window.GetWidth(), window.GetHeight());
}
NAMESPACE_END

