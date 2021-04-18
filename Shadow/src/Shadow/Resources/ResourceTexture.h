#pragma once
#include "Resource.h"

NAMESPACE_BEGAN
class Texture : public Resource
{
public:
	virtual uint32_t GetWidth();
	virtual uint32_t GetHeight();
	virtual void Bind(uint32_t slot = 0);
};

Texture* Create(std::string path);

NAMESPACE_END