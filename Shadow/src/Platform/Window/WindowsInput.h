#pragma once

#include "Shadow/Input.h"


NAMESPACE_BEGAN
class WindowsInput : public Input
{
public:

	virtual bool IsKeyPressedImpl(int keycode) override;
	virtual bool IsMouseButtonPressedImpl(int keycode) override;
	virtual std::pair<float, float> GetMousePositionImpl() override;
	virtual float GetMouseXImpl() override;
	virtual float GetMouseYImpl() override;

private:

};



NAMESPACE_END