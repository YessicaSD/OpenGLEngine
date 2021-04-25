#pragma once

#include "Shadow/Layers/Layer.h"

#include "Shadow/Event/EventMouse.h"
#include "Shadow/Event/EventKey.h"
#include "Shadow/Event/EventApplication.h"

NAMESPACE_BEGAN

class SHADOW_API LayerImGui : public Layer
{
public:
	LayerImGui();
	~LayerImGui();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	void Begin();
	void End();
	void StartTopBar();
	void EndTopBar();
private:



private:
	float m_Time = 0.0f;
};

NAMESPACE_END