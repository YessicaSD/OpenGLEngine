#pragma once

#include "Shadow/Renderer/Buffer.h"
#include "Shadow/Resources/Resource.h"

NAMESPACE_BEGAN
class Mesh : public Resource
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	unsigned int VAO;

	void SetupBuffers();

	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
};
NAMESPACE_END