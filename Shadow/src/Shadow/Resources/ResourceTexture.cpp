#include "swpch.h"
#include "ResourceTexture.h"
#include "Shadow/Layers/LayerRenderer.h"
#include "Shadow/Log.h"
#include "Platform/OpenGL/OpenGLTexture.h"

NAMESPACE_BEGAN

Texture* Create(std::string path)
{
	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
	case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTexture(path); break;
	}

	return nullptr;
}

NAMESPACE_END