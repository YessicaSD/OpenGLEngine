#pragma once
#include "Resource.h"

NAMESPACE_BEGAN

namespace TextureType
{
	enum TextureType : int
	{
		ALBEDO = 0,
		NORMAL = 1, 
		ROUGHNESS = 2,
		METAL = 3,
		AO = 4,
		MAX_TEXTURE = 5,
		
	};
}

class Texture : public Resource
{
public:
	~Texture() = default;
	virtual uint32_t GetWidth()  =0;
	virtual uint32_t GetHeight() = 0;
	virtual void Bind(uint32_t slot = 0) =0;
	virtual int GetHandle() { return 0; }
	virtual void GenerateMipmap() = 0;
};

NAMESPACE_END