#include "swpch.h"
#include "ResourceMaterial.h"
#include "Shadow/Log.h"
#include "Shadow/Layers/LayerResourceManager.h"

NAMESPACE_BEGAN

Material::Material()
{
	Init();
}

Material::~Material()
{
	program.reset();
	for (int i = 0; i < TextureType::MAX_TEXTURE; i++)
	{
			textures[i].reset();
			texturesActive[i] = false;
	}
}

Material::Material(std::string vs, std::string fs)
{
	program.reset(Shadow::Resources::CreateShader(vs, fs));
	Init();
}

void Material::UseMaterial()
{
	for (int i = 0; i < TextureType::MAX_TEXTURE; i++)
	{
		if(textures[i].use_count() > 0)
			textures[i]->Bind(i);
	}
}

void Material::SetTexture(TextureType::TextureType textureType, Texture* texture)
{
	switch (textureType)
	{
	case TextureType::ALBEDO:
		textures[TextureType::ALBEDO].reset(texture);

		break;
	case TextureType::NORMAL:
		textures[TextureType::NORMAL].reset(texture);
		break;
	case TextureType::ROUGHNESS:
		textures[TextureType::ROUGHNESS].reset(texture);
		break;
	case TextureType::METAL:
		textures[TextureType::METAL].reset(texture);
		break;
	default:
		SW_LOG_WARN("Could not set the texture");
		break;
	}

	if(textureType < TextureType::MAX_TEXTURE)
		texturesActive[textureType] = true;

}
void Material::SetActiveTextures(bool activeTex[4])
{
	//texturesActive = glm::vec4(activeTex[0], activeTex[1], activeTex[2], activeTex[3]);
}
void Material::Init()
{
	for (int i = 0; i < TextureType::MAX_TEXTURE; i++)
	{
		textures[i].reset();
		texturesActive[i] = false;
	}
}

NAMESPACE_END


