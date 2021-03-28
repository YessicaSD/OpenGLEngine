
#include <Shadow.h>
#include "imgui.h"

class Test : public Shadow::Layer
{
public:
	Test() : Layer("Test layer") {}

	void OnUpdate() override
	{
		if (Shadow::Input::IsKeyPressed(SW_KEY_0))
			SW_TRACE("0 IS PRESSED");
	}
	virtual void OnImGuiRender() override {
		ImGui::Begin("Hello");
		ImGui::End();
	};
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
