#pragma once
#include "Shadow/Core.h"
#include "Layer.h"

NAMESPACE_BEGAN

enum RenderAPI
{
	None,
	OpenGL,
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	static inline RenderAPI GetRendererAPI() { return rendererAPI; };

private:
	static RenderAPI rendererAPI;
};


NAMESPACE_END