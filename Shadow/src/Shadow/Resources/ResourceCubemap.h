#pragma once
#include "ResourceTexture.h"

NAMESPACE_BEGAN
class Cubemap : public Texture
{
public:

	virtual uint32_t GetWidth() = 0;
	virtual uint32_t GetHeight() = 0;
	virtual void Bind(uint32_t slot = 0) = 0;

	virtual void SetPositiveX(std::string path) = 0;
	virtual void SetNegativeX(std::string path) = 0;
	virtual void SetPositiveY(std::string path) = 0;
	virtual void SetNegativeY(std::string path) = 0;
	virtual void SetPositiveZ(std::string path) = 0;
	virtual void SetNegativeZ(std::string path) = 0;

};
NAMESPACE_END