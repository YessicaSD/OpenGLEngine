#include "swpch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Shadow/Log.h"
#include <glad/glad.h>
NAMESPACE_BEGAN

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case Shadow::ShaderDataType::FLOAT: 
	case Shadow::ShaderDataType::FLOAT2:
	case Shadow::ShaderDataType::FLOAT3:
	case Shadow::ShaderDataType::FLOAT4:
	case Shadow::ShaderDataType::MAT3:
	case Shadow::ShaderDataType::MAT4:
		return GL_FLOAT;
		break;

	case Shadow::ShaderDataType::INT:
	case Shadow::ShaderDataType::INT2:
	case Shadow::ShaderDataType::INT3:
	case Shadow::ShaderDataType::INT4:
		return GL_INT;
		break;
	case Shadow::ShaderDataType::BOOL:
		return GL_BOOL;
		break;

	case Shadow::ShaderDataType::NONE: default: return GL_NONE; 	break;
		}
}
OpenGLVertexArray::OpenGLVertexArray()
{
	glCreateVertexArrays(1, &vertexArrayID);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
	glDeleteVertexArrays(1, &vertexArrayID);
}


void OpenGLVertexArray::Bind() const
{
	glBindVertexArray(vertexArrayID);
}

void OpenGLVertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	SW_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

	glBindVertexArray(vertexArrayID);
	vertexBuffer->Bind();

	uint32_t index = 0;
	const auto& layout = vertexBuffer->GetLayout();

	for (const auto& element : layout)
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 
			element.GetElementCount(),
			ShaderDataTypeToOpenGLBaseType(element.type), 
			element.normalize ? GL_TRUE : GL_FALSE,
			layout.GetStride(),
			(const void*)element.offset);
		index++;
	}
	vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(vertexArrayID);
	indexBuffer->Bind();

	this->indexBuffer = indexBuffer;
	glBindVertexArray(0);
	indexBuffer->UnBind();
}

const std::vector<std::shared_ptr<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const
{
	return vertexBuffers;
}

const std::shared_ptr<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const
{
	return indexBuffer;
}

uint32_t OpenGLVertexArray::GetIndexCount()
{
	return indexBuffer->GetCount();
}


NAMESPACE_END
