#pragma once
#include "Shadow/Core/Core.h"
#include "glm/glm.hpp"
#include "VertexArray.h"

NAMESPACE_BEGAN

class RendererAPI
{
public:
	enum class RenderAPIType
	{
		NONE = 0, OPENGL = 1,
	};

public:
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void ClearScreen() = 0;

	virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

	inline static RenderAPIType GetAPI() { return api; };

private:
	static RenderAPIType api;
};
NAMESPACE_END