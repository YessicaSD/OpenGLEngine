#pragma once
#include "Shadow/Resources/ResourceTexture.h"

NAMESPACE_BEGAN

class OpenGLTexture : public Texture
{
public:
	OpenGLTexture(int width, int height, int internalFormat, int format, int type);
	OpenGLTexture(const std::string& path);
	/// <param name="mipLevelCount"> has to be greater than 0</param>
	OpenGLTexture(unsigned int* array, int width, int height, int layerCount = 2, unsigned int mipLevelCount = 1);
	OpenGLTexture(std::vector<glm::vec3>& array, int width, int height, int layerCount = 2, unsigned int mipLevelCount = 1);
	~OpenGLTexture();
	virtual uint32_t GetWidth() override;
	virtual uint32_t GetHeight() override;
	virtual void Bind(uint32_t slot) override;
	virtual int GetID() override;
private:
	int width = 0, height = 0;
	unsigned int textureID;
	std::string path;

};
NAMESPACE_END