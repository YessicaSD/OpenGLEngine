
#include "swpch.h"
#include "OpenGLPrimitive.h"

NAMESPACE_BEGAN

Model* OpenGLPrimitive::GetQuad()
{
	std::vector<Vertex> vertices = { Vertex(glm::vec3(-1.0f,  1.0f, 0.0f), glm::vec2(0.0f, 1.0f)),
									 Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)),
									 Vertex(glm::vec3(1.0f,  1.0f, 0.0f),  glm::vec2(1.0f, 1.0f)),
									 Vertex(glm::vec3(1.0f, -1.0f, 0.0f),  glm::vec2(1.0f, 0.0f)) };
	std::vector<unsigned int> indices = {0,1,2, 1,3,2};
	Mesh quadMesh = Mesh(vertices, indices);
	Model* ret = new Model();
	ret->AddMesh(quadMesh);
	return ret;
}

NAMESPACE_END


