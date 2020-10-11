#pragma once


#ifdef SW_PLATFORM_WINDOWS
extern Shadow::Application* Shadow::CreateApplication();
int main(int argc, char** argv)
{
	Shadow::Log::Init();
	SW_CORE_WARN("Initialized log!");
	int a = 5;
	SW_INFO("Initialized App! {}", a);


	auto app = Shadow::CreateApplication();
	app->Run();
	delete app;
}
#endif // SW_PLATFORM_WINDOWS
