#include "swpch.h"
#include "Entity.h"
#include "glm/gtx/euler_angles.hpp"
#include "Shadow/Resources/ResourceModel.h"

NAMESPACE_BEGAN

Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Material> material) : model(model), material(material)
{
	
}

void Entity::SetRotation(glm::vec3 rot)
{
	if (rotation == rot)
		return;

	rotation = rot;
	modelMatrix = glm::eulerAngleXYZ(rot.x, rot.y, rot.z);
	glm::scale(modelMatrix, glm::vec3(scale));
}

glm::vec3 Entity::GetRotation()
{
	return rotation;
}

void Entity::SetModel(glm::mat4 model)
{
	modelMatrix = model;
}

glm::mat4 Entity::GetModel()
{
	return modelMatrix;
}

void Entity::SetScale(float scale)
{
	this->scale = scale;
}

std::shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

void Entity::Draw()
{
	model->Draw();
}


NAMESPACE_END

