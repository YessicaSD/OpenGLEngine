#include "swpch.h"
#include "ResourceProgram.h"
#include "glad/glad.h"
#include "Shadow/Log.h"
#include <glm/gtc/type_ptr.hpp>
#include "Shadow/Layers/LayerRenderer.h"
#include "Platform/OpenGL/OpenGLProgram.h"

NAMESPACE_BEGAN

Program::~Program()
{
	Delete();
}

Program* CreateShader(std::string& vertexSource, std::string& fragmentSource)
{
	
	switch(Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:
			SW_CORE_ASSERT(false, "not render api using");
			return nullptr;
			break;

		case RendererAPI::RenderAPIType::OPENGL:
			return new OpenGLProgram(vertexSource, fragmentSource);
			break;
	
		default:
			break;
	}
}



NAMESPACE_END

