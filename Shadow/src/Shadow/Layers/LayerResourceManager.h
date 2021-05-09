#pragma once
#include "Shadow/Layers/Layer.h"
#include "Shadow/Resources/ResourceModel.h"
#include "Shadow/Resources/ResourceTexture.h"
#include "Shadow/Resources/ResourceCubemap.h"

NAMESPACE_BEGAN

class Resources: public Layer
{
public:
	Resources() {};
	~Resources() {};

	static Model* LoadModel(std::string path);
	static Model* GetQuad();
	static Texture* LoadTexture(std::string path);
	static Texture* CreateTextureFromArray(unsigned int* array, int width, int height, int layerCount = 2, int mipLevelCount = 1);
	static Cubemap* CreateCubemap();
	static std::shared_ptr<Texture> GetNoTextureTexture();
	virtual void OnImGuiRender() override;
	virtual void OnMainTopBar() override;

public:
	virtual void Init() override;
private:
	void CreateNoTextureTexture();

private:
	static Resources* instance;

	std::vector<Model*> Models;
	std::vector<Texture*> textures;

	std::shared_ptr<Texture> noTextureTex;
};

NAMESPACE_END