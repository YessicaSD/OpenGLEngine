#include "swpch.h"


#include <glad/glad.h>
#include <stb_image.h>

#include "OpenGLTextureCube.h"
#include "Shadow/Log.h"

#include "Shadow/Application.h"
#include "Shadow/Layers/LayerResourceManager.h"
#include "Shadow/Layers/LayerRenderer.h"


NAMESPACE_BEGAN

OpenGLTextureCube::~OpenGLTextureCube()
{
	glDeleteTextures(1, &textureID);
}

void OpenGLTextureCube::Init()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void OpenGLTextureCube::GenerateTexture()
{
	
}

void OpenGLTextureCube::SetCubeTexture(std::string& path, unsigned int target)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char* data;
	data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	glTexImage2D(
		target,
		0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
	);

	stbi_image_free(data);
}

OpenGLTextureCube::OpenGLTextureCube()
{
	Init();
}

OpenGLTextureCube::OpenGLTextureCube(Texture* texture)
{
	Init();
	int size = 512;
	for (unsigned int i = 0; i < 6; ++i)
	{
		// note that we store each face with 16 bit floating point values
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
			size, size, 0, GL_RGB, GL_FLOAT, nullptr);
	}

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
	
	Resources::instance->bakeFBO->Bind();
	Resources::instance->bakeRBO->Bind();
	Resources::instance->bakeRBO->DefineDepthStorageSize(512);
	Resources::instance->bakeRBO->BindDepthToFrameBuffer();

	texture->Bind(0);
	Resources::instance->cubeToTexture->Bind();
	Resources::instance->cubeToTexture->UploadUniformMat4("projection", captureProjection);
	Renderer::SetViewPort(0, 0, 512, 512);
	
	for (unsigned int i = 0; i < 6; ++i)
	{
		Resources::instance->cubeToTexture->UploadUniformMat4("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, textureID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Resources::instance->cubeModel->Draw();
	}

	Window& window = Application::Get().GetWindow();

	Renderer::SetViewPort(0, 0, window.GetWidth(), window.GetHeight());
}


uint32_t OpenGLTextureCube::GetWidth()
{
	return width;
}

uint32_t OpenGLTextureCube::GetHeight()
{
	return height;
}

void OpenGLTextureCube::Bind(uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

int OpenGLTextureCube::GetHandle()
{
	return textureID;
}

void OpenGLTextureCube::SetPositiveX(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
}

void OpenGLTextureCube::SetNegativeX(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
}

void OpenGLTextureCube::SetPositiveY(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
}

void OpenGLTextureCube::SetNegativeY(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
}

void OpenGLTextureCube::SetPositiveZ(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
}

void OpenGLTextureCube::SetNegativeZ(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}


NAMESPACE_END

