#pragma once

#include "Shadow/Layers/Input.h"
#include "Shadow/Event/EventMouse.h"

NAMESPACE_BEGAN
class WindowsInput : public Input
{
public:
	WindowsInput();
	virtual bool IsKeyPressedImpl(int keycode) override;
	virtual bool IsMouseButtonPressedImpl(int keycode) override;
	virtual std::pair<float, float> GetMousePositionImpl() override;
	virtual float GetMouseXImpl() override;
	virtual float GetMouseYImpl() override;
	virtual float GetScrollXImpl() override;
	virtual float GetScrollYImpl() override;
	virtual void OnEvent(Event& e) override;
	virtual void EndFrame() override;
private:
	bool ScrollDispatch(MouseScrolledEvent& e);
	static glm::vec2 mouseScrollDelta;
};



NAMESPACE_END