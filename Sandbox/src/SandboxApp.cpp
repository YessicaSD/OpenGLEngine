
#include <Shadow.h>

class Sandbox : public Shadow::Application
{
public:
	Sandbox() 
	{
		PushLayer(new Shadow::LayerImGui());
	};
	~Sandbox() {};

private:

};

Shadow::Application* Shadow::CreateApplication()
{
	return new Sandbox();
}
