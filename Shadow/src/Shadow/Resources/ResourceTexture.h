#pragma once
#include "Resource.h"

NAMESPACE_BEGAN

namespace TextureType
{
	enum TextureType : int
	{
		ALBEDO = 0,
		NORMAL = 1,
		AMBIENT_OCCLUSION = 2,
		ROUGHNESS = 3,
		SPECULAR = 4,
		MAX_TEXTURE,
	};
}

class Texture : public Resource
{
public:
	virtual uint32_t GetWidth()  =0;
	virtual uint32_t GetHeight() = 0;
	virtual void Bind(uint32_t slot = 0) =0;
};

NAMESPACE_END