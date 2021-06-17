#include "swpch.h"
#include "Light.h"

NAMESPACE_BEGAN

PointLight::PointLight(glm::vec3 position, glm::vec3 color) :Light(LightTypes::POINT, position, color)
{}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 position, glm::vec3 color) :
direction(direction), 
Light (LightTypes::POINT, position, color)
{}

Light::Light(Shadow::LightTypes type, glm::vec3 position, glm::vec3 color) : type(type), color(color), position(position)
{
}

NAMESPACE_END


