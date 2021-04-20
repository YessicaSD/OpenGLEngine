#pragma once
#include "ResourceTexture.h"

NAMESPACE_BEGAN
class Cubemap : public Texture
{
public:
	Cubemap();
	~Cubemap();

	virtual uint32_t GetWidth() override;
	virtual uint32_t GetHeight() override;
	virtual void Bind(uint32_t slot = 0) override;

	void SetPositiveX(std::string path);
	void SetNegativeX(std::string path);
	void SetPositiveY(std::string path);
	void SetNegativeY(std::string path);
	void SetPositiveZ(std::string path);
	void SetNegativeZ(std::string path);

private:
	void SetCubeTexture(std::string& path, unsigned int target);
	unsigned int textureID;
};
NAMESPACE_END