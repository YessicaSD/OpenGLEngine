#pragma once
#include "Shadow/Resources/ResourceModel.h"
#include "Shadow/Resources/ResourceMaterial.h"
NAMESPACE_BEGAN
class Entity
{
public:
	Entity(std::shared_ptr<Model> model, std::shared_ptr<Material> material);

	void SetRotation(glm::vec3 rotation);
	glm::vec3 GetRotation();
	void SetModel(glm::mat4 model);
	glm::mat4 GetModel();
	void SetScale(float scale);
	std::shared_ptr<Material> GetMaterial();
	void Draw();

private:
	std::shared_ptr<Model> model;
	std::shared_ptr<Material> material;
	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::vec3 rotation = glm::vec3(0.0);
	float scale = 1.0;
};
NAMESPACE_END