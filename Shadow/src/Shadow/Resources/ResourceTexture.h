#pragma once
#include "Resource.h"

NAMESPACE_BEGAN
class Texture : public Resource
{
public:
	virtual uint32_t GetWidth()  =0;
	virtual uint32_t GetHeight() = 0;
	virtual void Bind(uint32_t slot = 0) =0;
};

NAMESPACE_END