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
	bool* GetActiveTextures() {
		return &texturesActive[0];
	};
	void SetActiveTextures(bool activeTex[4]);
	void SetColor(float color[3]);

	glm::vec3& GetColor() { return color; };
	glm::vec2& GetRoughnessMetalness() { return rmData; };

private:
	void Init();
private:
	std::shared_ptr<Texture> textures[TextureType::MAX_TEXTURE];
	std::shared_ptr<Program> program;
	std::string vs;
	std::string fs;
	glm::vec3 color = {0,0,0};
	glm::vec2 rmData = { 0.5,0.0 };
	bool texturesActive[TextureType::MAX_TEXTURE];

	friend class LayerRenderer;
};

NAMESPACE_END