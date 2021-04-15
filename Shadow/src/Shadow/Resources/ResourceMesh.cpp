#include "swpch.h"
#include "ResourceMesh.h"
#include "Shadow/Layers/LayerRenderer.h"

NAMESPACE_BEGAN
Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, glm::mat4 matrix): transform(matrix)
{
	this->vertices = vertices;
	this->indices = indices;

	SetupBuffers();
}

void Mesh::SetupBuffers()
{
	std::shared_ptr<VertexBuffer> vertexBuffer;
	vertexBuffer.reset(VertexBuffer::Create(&vertices[0], vertices.size()));
	std::shared_ptr<IndexBuffer> indexBuffer;
	indexBuffer.reset(IndexBuffer::Create(&indices[0], indices.size()));

	vertexArray.reset(VertexArray::Create());

	vertexBuffer->SetLayout({
		{ShaderDataType::FLOAT3, "aPos"},
		{ShaderDataType::FLOAT3, "a_Normal"},
		{ShaderDataType::FLOAT2, "a_UV"}
		});

	vertexArray->AddVertexBuffer(vertexBuffer);
	vertexArray->SetIndexBuffer(indexBuffer);
}

void Mesh::Draw()
{
	if (!vertexArray)
		return;

	
}

NAMESPACE_END


