#pragma once

#include "Shadow/Renderer/Buffer.h"
#include "Shadow/Renderer/VertexArray.h"
#include "Shadow/Resources/Resource.h"

NAMESPACE_BEGAN
class Mesh : public Resource
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	void Draw();

private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void SetupBuffers();
	
	std::shared_ptr<VertexArray> vertexArray;

};
NAMESPACE_END