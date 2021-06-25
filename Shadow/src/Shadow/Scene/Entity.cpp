#include "swpch.h"
#include "Entity.h"
#include "glm/gtx/euler_angles.hpp"
#include "Shadow/Resources/ResourceModel.h"

NAMESPACE_BEGAN

Entity::Entity(std::shared_ptr<Model> model, std::shared_ptr<Material> material, std::string name) : 
	model(model), material(material), name(name)
{
	
}

void Entity::SetRotation(glm::vec3 rot)
{
	if (rotation == rot)
		return;

	rotation = rot;
	UpdateMatrix();
}

void Entity::SetPosition(glm::vec3 pos)
{
	if (pos == position)
		return;

	position = pos;
	UpdateMatrix();
}

glm::vec3 Entity::GetPosition()
{
	return position;
}

glm::vec3 Entity::GetRotation()
{
	return rotation;
}

glm::vec3 Entity::GetScale()
{
	return scale;
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
	SetScale(glm::vec3(scale));
}

void Entity::SetScale(glm::vec3 scale)
{
	if (scale == this->scale)
		return;
	this->scale = scale;
	UpdateMatrix();
}

std::shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

void Entity::Draw()
{
	model->Draw();
}

void Entity::UpdateMatrix()
{
	modelMatrix = glm::eulerAngleXYZ(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);

}


NAMESPACE_END

