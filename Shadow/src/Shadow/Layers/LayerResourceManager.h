#pragma once
#include "Shadow/Core.h"
#include "Shadow/Layers/Layer.h"
NAMESPACE_BEGAN

class LayerResourceManager: public Layer
{
public:
	LayerResourceManager();
	~LayerResourceManager();

	void LoadScene();

private:

};

NAMESPACE_END