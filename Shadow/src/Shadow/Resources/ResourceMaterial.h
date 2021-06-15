#pragma once
#include "Resource.h"
#include "ResourceProgram.h"
#include "ResourceTexture.h"
NAMESPACE_BEGAN



class Material : public Resource
{
public:
	Material();
	~Material();
	Material(std::string vs, std::string fs);
	void UseMaterial();
	std::shared_ptr<Program> GetProgram() { return program; };
	void SetTexture(TextureType::TextureType textureType, Texture* texture);

private:
	void Init();
private:
	std::shared_ptr<Texture> textures[TextureType::MAX_TEXTURE];
	std::shared_ptr<Program> program;
	std::string vs;
	std::string fs;
};

NAMESPACE_END