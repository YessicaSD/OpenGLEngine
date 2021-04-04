#include "swpch.h"
#include "ResourceMesh.h"

NAMESPACE_BEGAN
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	SetupBuffers();
}

void Mesh::SetupBuffers()
{
	vertexBuffer.reset(VertexBuffer::Create(&vertices[0], vertices.size()));
	indexBuffer.reset(IndexBuffer::Create(&indices[0], indices.size()));
	
}

NAMESPACE_END


