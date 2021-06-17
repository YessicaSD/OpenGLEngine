#pragma once

NAMESPACE_BEGAN

enum class LightTypes : int
{
	DIRECTIONAL = 0,
	POINT,
};

class Light
{
public:
	Light(Shadow::LightTypes type, glm::vec3 position, glm::vec3 color);
	LightTypes type;
	glm::vec3 color;
	glm::vec3 position;
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position, glm::vec3 color = {1.0,1.0,1.0});
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 direction, glm::vec3 position = {0,0,0}, glm::vec3 color = { 1,1,1 });
	glm::vec3 direction;
};

NAMESPACE_END