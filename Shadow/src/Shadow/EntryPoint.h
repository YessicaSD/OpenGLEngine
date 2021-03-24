#pragma once


#ifdef SW_PLATFORM_WINDOWS
extern Shadow::Application* Shadow::CreateApplication();
int main(int argc, char** argv)
{
	Shadow::Log::Init();
	SW_INFO("Initialized App!");


	auto app = Shadow::CreateApplication();
	app->Run();
	delete app;
}

#endif // SW_PLATFORM_WINDOWS
