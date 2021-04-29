#include "swpch.h"
#include "OpenGLTexture.h"
#include <stb_image.h>
#include <glad/glad.h>

#include "Shadow/Log.h"

NAMESPACE_BEGAN
OpenGLTexture::OpenGLTexture(const std::string& path)
	: path(path)
{
	int channels;
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	SW_ASSERT(data, "Failed to load image!");

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if(channels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	

	stbi_image_free(data);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &textureID);
}

uint32_t OpenGLTexture::GetWidth()
{
	return uint32_t();
}

uint32_t OpenGLTexture::GetHeight()
{
	return uint32_t();
}

void OpenGLTexture::Bind(uint32_t slot)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

NAMESPACE_END


