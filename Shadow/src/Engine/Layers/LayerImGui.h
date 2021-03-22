#pragma once

#include "Engine/Layers/Layer.h"

#include "Engine/Core.h"
#include "Engine/Event/EventMouse.h"
#include "Engine/Event/EventKey.h"
#include "Engine/Event/EventApplication.h"

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
	//Mouse Event
	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	bool OnMouseMovedEvent(MouseMovedEvent& e);
	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	//Key Event
	bool OnKeyPressedEvent(KeyPressedEvent& e);
	bool OnKeyReleasedEvent(KeyReleasedEvent& e);
	bool OnKeyTypedEvent(KeyTypedEvent& e);
	// Windows Event
	bool OnWindowResizedEvent(WindowResizedEvent& e);


private:
	float m_Time = 0.0f;
};

NAMESPACE_END