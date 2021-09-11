#include "swpch.h"
#include "Time.h"

#include "GLFW/glfw3.h"

NAMESPACE_BEGAN

float Time::deltaTime = 0.0f;

void Shadow::Time::CalculateDeltaTime()
{
	float time = (float)glfwGetTime();
	deltaTime = time - lastFrameTime;
	lastFrameTime = time;
}

NAMESPACE_END