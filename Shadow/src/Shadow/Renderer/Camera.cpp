#include "swpch.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

NAMESPACE_BEGAN

void Camera::UpdateViewTransformation()
{
	if (orbit)
	{
		UpdateViewTransformOrbit();
	}
	else
	{

	}
}

Camera::Camera(float fov, float aspectRatio, float nearPlan, float farPlane)
{
	position = { 0,1,3 };
	projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlan, farPlane);
	SetOrbit({ 0,0,0 });
}

void Camera::SetOrbit(glm::vec3 lookPosition)
{
	orbit = true;
	this->lookPosition = lookPosition;
	UpdateViewTransformOrbit();
}

glm::mat4 Camera::GetProjectViewMatrix() const 
{
	return viewProjMatrix;
}

glm::mat4 Camera::GetViewMatrix() const 
{
	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const 
{
	return projectionMatrix;
}

void Camera::SetPosition(const glm::vec3 position)
{
	this->position = position;
	UpdateViewTransformation();
}

void Camera::SetRotation(glm::vec3 rotation)
{
}

void Camera::UpdateViewTransformOrbit()
{
	//forward===
	forward = glm::normalize(position - lookPosition);
	//right ===
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(worldUp, forward));
	//up ===
	up = glm::cross(forward, right);

	viewMatrix = glm::lookAt (position,
								lookPosition,
								worldUp);
	viewProjMatrix = projectionMatrix * viewMatrix;
}

void Camera::UpdateViewTransformationFreeOrbit()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);
	viewMatrix = glm::inverse(transform);
}

NAMESPACE_END
