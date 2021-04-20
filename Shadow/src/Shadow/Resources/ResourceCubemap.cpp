#include "swpch.h"
#include "ResourceCubemap.h"
#include <glad/glad.h>
#include "stb_image.h"


NAMESPACE_BEGAN

Cubemap::Cubemap()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap()
{
	glDeleteBuffers(1, &textureID);
}

uint32_t Cubemap::GetWidth()
{
	return uint32_t();
}

uint32_t Cubemap::GetHeight()
{
	return uint32_t();
}

void Cubemap::Bind(uint32_t slot)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

void Cubemap::SetPositiveX(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
}

void Cubemap::SetNegativeX(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
}

void Cubemap::SetPositiveY(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
}

void Cubemap::SetNegativeY(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
}

void Cubemap::SetPositiveZ(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
}

void Cubemap::SetNegativeZ(std::string path)
{
	SetCubeTexture(path, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}

void Cubemap::SetCubeTexture(std::string& path, unsigned int target)
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

NAMESPACE_END


