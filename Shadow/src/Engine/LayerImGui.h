#pragma once

#include "Engine/Layer.h"

#include "Core.h"

NAMESPACE_BEGAN

class SHADOW_API LayerImGui : public Layer
{
public:
	LayerImGui();
	~LayerImGui();

	void OnUpdate() override;
	void OnEvent(Event& event) override;

	void OnAttach() override;
	void OnDetach() override;
private:
	float m_Time = 0.0f;
};

NAMESPACE_END