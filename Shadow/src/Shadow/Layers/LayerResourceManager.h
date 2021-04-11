#pragma once
#include "Shadow/Layers/Layer.h"
#include "Shadow/Resources/ResourceModel.h"

NAMESPACE_BEGAN

class Resources: public Layer
{
public:
	Resources();
	~Resources();

	static Model* LoadScene(std::string path);

private:
	static Resources* instance;

	std::vector<Model*> Models;
};

NAMESPACE_END