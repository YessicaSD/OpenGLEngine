
#include <Shadow.h>

class Sandbox : public Shadow::Application
{
public:
	Sandbox() {};
	~Sandbox() {};

private:

};
Shadow::Application* Shadow::CreateApplication()
{
	return new Sandbox();
}
