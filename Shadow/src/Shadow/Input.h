#pragma once
#include "KeyCodes.h"
NAMESPACE_BEGAN

// this class is an interphase so for diferent platforms we are going to define diferent ways to read input

class SHADOW_API Input
{
public:
	//TODO Maybe add key repeat and key up and key down
	static inline bool IsKeyPressed(int keycode) { return instance->IsKeyPressedImpl(keycode); }
	inline bool IsMouseButtonPressed(int keycode) { return instance->IsMouseButtonPressedImpl(keycode); }
	inline static std::pair<float, float> GetMousePosition() { return instance->GetMousePositionImpl(); }
	inline float GetMouseX() { return instance->GetMouseXImpl(); }
	inline float GetMouseY() { return instance->GetMouseYImpl(); }
protected:
	// implementation of this functions will be in childs
	virtual bool IsKeyPressedImpl(int keycode) = 0;
	virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
	virtual std::pair<float, float> GetMousePositionImpl() = 0;
	virtual float GetMouseXImpl() = 0;
	virtual float GetMouseYImpl() = 0;
	
private:
	static Input* instance;
};


NAMESPACE_END