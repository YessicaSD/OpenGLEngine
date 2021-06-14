#pragma once

#pragma once
#include "Shadow/Resources/ResourceCubemap.h"

NAMESPACE_BEGAN

class OpenGLTextureCube : public Cubemap
{
public:
	OpenGLTextureCube();
	OpenGLTextureCube(Texture* texture);
	~OpenGLTextureCube();

	virtual uint32_t GetWidth() override;
	virtual uint32_t GetHeight() override;
	virtual void Bind(uint32_t slot) override;
	virtual int GetHandle() override;

	virtual void SetPositiveX(std::string path);
	virtual void SetNegativeX(std::string path);
	virtual void SetPositiveY(std::string path);
	virtual void SetNegativeY(std::string path);
	virtual void SetPositiveZ(std::string path);
	virtual void SetNegativeZ(std::string path);

	virtual void SetTexturesSize(int width, int height) override;
private:
	void Init();
	void GenerateTexture();
	void SetCubeTexture(std::string& path, unsigned int target);

private:
	int width = 0, height = 0;
	unsigned int textureID;
	std::string path;

};

NAMESPACE_END