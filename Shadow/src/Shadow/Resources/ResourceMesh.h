#pragma once

#include "Shadow/Renderer/Buffer.h"
#include "Shadow/Renderer/VertexArray.h"
#include "Shadow/Resources/Resource.h"
#include "glm/mat4x4.hpp"

NAMESPACE_BEGAN
class Mesh : public Resource
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, glm::mat4 matrix = glm::mat4(1.0));
	void Draw();
	std::string GetName() const { return name; };

private:
	std::string name;
	glm::mat4 transform;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	void SetupBuffers();
	

	std::shared_ptr<VertexArray> vertexArray;
	friend class Model;
};
NAMESPACE_END