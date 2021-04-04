#include "swpch.h"
#include "Buffer.h"
#include "Shadow/Layers/LayerRenderer.h"
#include "Shadow/Log.h"
#include "Shadow/Platform/OpenGL/OpenGLBuffer.h"

NAMESPACE_BEGAN
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
{
	switch (Renderer::GetRendererAPI())
	{
	case RenderAPI::None:
		SW_CORE_ASSERT(false, "not render api using");
		return nullptr;
		break;

	case RenderAPI::OpenGL:
		return new OpenGLVertexBuffer(vertices, size);
		break;

	default:
		break;
	}
	return nullptr;
}

VertexBuffer* VertexBuffer::Create(Vertex* vertices, uint32_t count)
{
	switch (Renderer::GetRendererAPI())
	{
	case RenderAPI::None:
		SW_CORE_ASSERT(false, "not render api using");
		return nullptr;
		break;

	case RenderAPI::OpenGL:
		return new OpenGLVertexBuffer(vertices, count);
		break;

	default:
		break;
	}
	return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
{
	switch (Renderer::GetRendererAPI())
	{
		case RenderAPI::None:
			SW_CORE_ASSERT(false, "not render api using");
			return nullptr;
			break;

		case RenderAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);
			break;

		default:
			break;
	}

	return nullptr;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements) :
			  elements(elements)
{
	CalculateOffsetAndStrinde();
}

void BufferLayout::CalculateOffsetAndStrinde()
{
	uint32_t offset = 0;
	stride = 0;
	for (auto& element : elements)
	{
		element.offset = offset;
		offset += element.size;
		stride += element.size;
	}
}

NAMESPACE_END

