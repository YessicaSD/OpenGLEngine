#pragma once
#include "Shadow/Renderer/RendererAPI.h"

NAMESPACE_BEGAN

class OpenGLRendererAPI : public RendererAPI
{
public:
	virtual void SetClearColor(const glm::vec4& color) override;
	virtual void ClearScreen() override;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;

private:
	glm::vec4 clearColor;
};
NAMESPACE_END