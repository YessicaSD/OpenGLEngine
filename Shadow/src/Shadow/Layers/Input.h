#pragma once

#include "Layer.h"

#include "Shadow/Core/KeyCodes.h"
#include "Shadow/Core/MouseButtonCodes.h"

#include "Shadow/Core/Core.h"
#include "glm/vec2.hpp"

NAMESPACE_BEGAN

// this class is an interphase so for diferent platforms we are going to define diferent ways to read input

class SHADOW_API Input : public Layer
{
public:
	//TODO Maybe add key repeat and key up and key down
	static inline bool IsKeyPressed(int keycode) { return instance->IsKeyPressedImpl(keycode); }
	static inline bool IsMouseButtonPressed(int keycode) { return instance->IsMouseButtonPressedImpl(keycode); }
	static inline std::pair<float, float> GetMousePosition() { return instance->GetMousePositionImpl(); }
	static inline float GetMouseX() { return instance->GetMouseXImpl(); }
	static inline float GetMouseY() { return instance->GetMouseYImpl(); }
	static inline float GetScrollX() { return instance->GetScrollXImpl(); };
	static inline float GetScrollY() { return instance->GetScrollYImpl(); };
protected:
	// implementation of this functions will be in childs
	virtual bool IsKeyPressedImpl(int keycode) = 0;
	virtual bool IsMouseButtonPressedImpl(int keycode) = 0;
	virtual std::pair<float, float> GetMousePositionImpl() = 0;
	virtual float GetMouseXImpl() = 0;
	virtual float GetMouseYImpl() = 0;
	virtual float GetScrollXImpl() = 0;
	virtual float GetScrollYImpl() = 0;
	
private:
	
	static Input* instance;
};


NAMESPACE_END