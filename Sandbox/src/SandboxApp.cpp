
#include <Shadow.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi


class Test : public Shadow::Layer
{
public:
	Test() : Layer("Test layer")
	{
		
	}

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
		PushOverlay(new Shadow::LayerImGui());

	};
	~Sandbox() {};

private:

};

Shadow::Application* Shadow::CreateApplication()
{
	return new Sandbox();
}
