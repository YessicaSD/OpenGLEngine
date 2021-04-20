#pragma once
#include "Shadow/Layers/Layer.h"
#include "Shadow/Resources/ResourceModel.h"
#include "Shadow/Resources/ResourceTexture.h"
#include "Shadow/Resources/ResourceCubemap.h"

NAMESPACE_BEGAN

class Resources: public Layer
{
public:
	Resources();
	~Resources();

	static Model* LoadModel(std::string path);
	static Texture* LoadTexture(std::string path);
	static Cubemap* CreateCubemap();

private:
	static Resources* instance;

	std::vector<Model*> Models;
	std::vector<Texture*> textures;
};

NAMESPACE_END