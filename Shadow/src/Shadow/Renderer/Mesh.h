#pragma once
#include "Shadow/Core.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

NAMESPACE_BEGAN
struct Vertex
{
	glm::vec3 position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<Vertex>& indices);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	unsigned int VAO, VBO, EBO;

	void SetupMesh();

};
NAMESPACE_END