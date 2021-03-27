
#include <Shadow.h>


class Test : public Shadow::Layer
{
public:
	Test() : Layer("Test layer") {}

	void OnUpdate() override
	{
		if (Shadow::Input::IsKeyPressed(SW_KEY_0))
			SW_TRACE("0 IS PRESSED");
	}
};
class Sandbox : public Shadow::Application
{
public:
	Sandbox() 
	{
		PushLayer(new Test());
	};
	~Sandbox() {};

private:

};

Shadow::Application* Shadow::CreateApplication()
{
	return new Sandbox();
}
