#include "swpch.h"
#include "VertexArray.h"
#include "Shadow/Layers/LayerRenderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Shadow/Log.h"
NAMESPACE_BEGAN

VertexArray* VertexArray::Create()
{
	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::RenderAPIType::NONE:
		SW_CORE_ASSERT(false, "not render api using");
		return nullptr;
		break;

	case RendererAPI::RenderAPIType::OPENGL:
		return new OpenGLVertexArray();
		break;

	default:
		break;
	}
	return nullptr;
}

NAMESPACE_END