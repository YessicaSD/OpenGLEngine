#pragma once
#include "Layer.h"

NAMESPACE_BEGAN

class MaterialEditor : public Layer
{
public:
	MaterialEditor();
	~MaterialEditor();

	virtual void OnImGuiRender() override;
private:
	
};

NAMESPACE_END