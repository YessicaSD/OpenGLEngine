#include "swpch.h"

#include "Shadow/Input.h"
#include "WindowsInput.h"

#include "Shadow/Application.h"

#include <GLFW/glfw3.h>

NAMESPACE_BEGAN

//TODO this is weird, ask later
Input* Input::instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetKey(window, keycode);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int keycode)
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	auto state = glfwGetMouseButton(window, keycode);
	return state == GLFW_PRESS;

	return false;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl()
{
	auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return { (float)xpos , (float)ypos};
}

float WindowsInput::GetMouseXImpl()
{
	return (float)GetMousePositionImpl().first;
}

float WindowsInput::GetMouseYImpl()
{
	return (float)GetMousePositionImpl().second;
}

NAMESPACE_END