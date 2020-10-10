#pragma once
#include <stdio.h>

#ifdef SW_PLATFORM_WINDOWS
extern Shadow::Application* Shadow::CreateApplication();
int main(int argc, char** argv)
{
	printf("Welcome to the Shadow Engine");
	auto app = Shadow::CreateApplication();
	app->Run();
	delete app;
}
#endif // SW_PLATFORM_WINDOWS
