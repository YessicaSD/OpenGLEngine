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
	glTextureStorage2D(textureID, 1, GL_RGB8, width, height);

	glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureSubImage2D(textureID, 0,0,0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}

OpenGLTexture::~OpenGLTexture()
{
	glDeleteTextures(1, &textureID);
}

void OpenGLTexture::Bind(uint32_t slot)
{
	glBindTextureUnit(slot, textureID);
}

NAMESPACE_END


