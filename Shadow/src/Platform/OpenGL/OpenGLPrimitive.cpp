
#include "swpch.h"
#include "OpenGLPrimitive.h"

NAMESPACE_BEGAN

Model* OpenGLPrimitive::GetQuad()
{
	std::vector<Vertex> vertices = { Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
									 Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
									 Vertex(glm::vec3(1.0f,  1.0f, 0.0f),  glm::vec2(1.0f, 1.0f)),
									 Vertex(glm::vec3(1.0f, -1.0f, 0.0f),  glm::vec2(1.0f, 0.0f)) };
	std::vector<unsigned int> indices = { 0,1,2, 1,3,2 };
	Mesh quadMesh = Mesh(vertices, indices);
	Model* ret = new Model();
	ret->AddMesh(quadMesh);
	return ret;
}

Model* OpenGLPrimitive::GetCube()
{
	std::vector<Vertex> vertices = { Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 0)),
									 Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 0)),
									 Vertex(glm::vec3(1, 1, -1),  glm::vec2(1, 1)),
									 Vertex(glm::vec3(-1, 1, -1),  glm::vec2(0, 1)),
									 Vertex(glm::vec3(-1, -1, 1), glm::vec2(0, 0)),
									 Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0)),
									 Vertex(glm::vec3(1, 1, 1),  glm::vec2(1, 1)),
									 Vertex(glm::vec3(-1, 1, 1),  glm::vec2(0, 1))
	};

	std::vector<unsigned int> indices = { 0, 1, 3, 3, 1, 2,
										  1, 5, 2, 2, 5, 6,
										  5, 4, 6, 6, 4, 7,
										  4, 0, 7, 7, 0, 3,
										  3, 2, 7, 7, 2, 6,
										  4, 5, 0, 0, 5, 1 };
	
	Mesh quadMesh = Mesh(vertices, indices);
	Model* ret = new Model();
	ret->AddMesh(quadMesh);
	return ret;
}

NAMESPACE_END


