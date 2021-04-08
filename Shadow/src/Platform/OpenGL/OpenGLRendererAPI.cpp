#include "swpch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>

NAMESPACE_BEGAN
void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
{
	clearColor = color;
}

void OpenGLRendererAPI::ClearScreen()
{
	glClearColor(clearColor.r, clearColor.g,clearColor.b, clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
{
	glDrawElements(GL_TRIANGLES, vertexArray->GetIndexCount(), GL_UNSIGNED_INT, 0);
}

NAMESPACE_END