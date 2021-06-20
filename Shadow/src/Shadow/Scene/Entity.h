#pragma once
#include "Shadow/Resources/ResourceModel.h"
#include "Shadow/Resources/ResourceMaterial.h"
NAMESPACE_BEGAN
class Entity
{
public:
	Entity(std::shared_ptr<Model> model, std::shared_ptr<Material> material);

	void SetRotation(glm::vec3 rotation);
	void SetPosition(glm::vec3 position);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();
	glm::mat4 GetModel();

	void SetModel(glm::mat4 model);

	void SetScale(float scale);
	void SetScale(glm::vec3 scale);
	std::shared_ptr<Material> GetMaterial();
	void Draw();
private:
	void UpdateMatrix();

private:
	std::shared_ptr<Model> model;
	std::shared_ptr<Material> material;
	glm::mat4 modelMatrix = glm::mat4(1.0);
	glm::vec3 rotation = glm::vec3(0.0);
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 scale = glm::vec3(1.0);
};
NAMESPACE_END