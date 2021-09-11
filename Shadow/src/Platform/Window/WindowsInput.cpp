#include "swpch.h"

#include "Shadow/Layers/Input.h"
#include "WindowsInput.h"

#include "Shadow/Application.h"

#include <GLFW/glfw3.h>

#include "Shadow/Core/Core.h"


NAMESPACE_BEGAN

//This is how it's intialiced static variables
Input* Input::instance = new WindowsInput();
glm::vec2 WindowsInput::mouseScrollDelta = { 0.f,0.f };

WindowsInput::WindowsInput() 
{
	this->debugName = "Input";
}

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

float WindowsInput::GetScrollXImpl()
{
	return this->mouseScrollDelta.x;
}

float WindowsInput::GetScrollYImpl()
{
	return this->mouseScrollDelta.y;
}

void WindowsInput::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<MouseScrolledEvent>(SW_BIND_FN(WindowsInput::ScrollDispatch));
	
}

void WindowsInput::EndFrame()
{
	mouseScrollDelta = { 0.f,0.f };
}

bool WindowsInput::ScrollDispatch(MouseScrolledEvent& e)
{
	this->mouseScrollDelta.x = e.GetXOffset();
	this->mouseScrollDelta.y = e.GetYOffset();
	SW_LOG_INFO("y scroll {0}", mouseScrollDelta.y);

	return false;
}

NAMESPACE_END