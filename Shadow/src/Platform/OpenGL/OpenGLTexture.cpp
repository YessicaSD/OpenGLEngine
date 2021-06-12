#include "swpch.h"
#include "glm/vec3.hpp"
#include "OpenGLTexture.h"
#include <stb_image.h>
#include <glad/glad.h>

#include "Shadow/Log.h"

NAMESPACE_BEGAN
OpenGLTexture::OpenGLTexture(const std::string& path)
	: path(path)
{
	int channels;
	void* data = nullptr;
	bool isHDR = stbi_is_hdr(path.c_str());
	if (isHDR)
	{
		stbi_set_flip_vertically_on_load(true);
		data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
	}
	else
	{
		stbi_set_flip_vertically_on_load(false);
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	}

	if (!data)
		SW_LOG_ERROR("Failed to load image!" + path);

	
	//SW_ASSERT(data, "Failed to load image!");

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, isHDR ? GL_RGB16F : GL_RGBA, width, height, 0, channels == 4 ? GL_RGBA : GL_RGB,
				 isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE, data);

	//glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

OpenGLTexture::OpenGLTexture(int w, int h, int internalFormat, int format, int type)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	width = w;
	height = h;
}

OpenGLTexture::OpenGLTexture(unsigned int* array, int width, int height, int layerCount, unsigned int mipLevelCount)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Allocate the storage.
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);

	// Upload pixel data.
	// The first 0 refers to the mipmap level (level 0, since there's only 1)
	// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
	// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
	// Altogether you can specify a 3D box subset of the overall texture, but only one mip level at a time.
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount-1, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, array);

	// Always set reasonable texture parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_2D);

	this->width = width;
	this->height = height;
}

OpenGLTexture::OpenGLTexture(std::vector<glm::vec3>& array, int width, int height, int layerCount, unsigned int mipLevelCount)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Allocate the storage.
	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, mipLevelCount, GL_RGBA8, width, height, layerCount);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &array[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

int OpenGLTexture::GetHandle()
{
	return textureID;
}

NAMESPACE_END


