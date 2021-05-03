#include "swpch.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include  <imgui.h>

NAMESPACE_BEGAN

void Camera::UpdateViewTransformation()
{
	if (orbit)
	{
		UpdateViewTransformOrbit();
	}
	else
	{
		UpdateViewTransformationLookAround();
	}
}

Camera::Camera(float fov, float aspectRatio, float nearPlan, float farPlane)
{
	projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearPlan, farPlane);
	UpdateViewTransformationLookAround();
	//SetOrbit({ 0,0,0 });
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

void Camera::OnImGuiRender()
{
	ImGui::Text("Camera information");
	ImGui::Text("Position:");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("x", &position.x);  ImGui::SameLine();
	ImGui::DragFloat("y", &position.y); ImGui::SameLine();
	ImGui::DragFloat("z", &position.z);
	ImGui::PopItemWidth();

	ImGui::Text("Rotation:");
	ImGui::PushItemWidth(100);
	ImGui::DragFloat("pitch", &rotation.x);  ImGui::SameLine();
	ImGui::DragFloat("yaw", &rotation.y); ImGui::SameLine();
	ImGui::DragFloat("roll", &rotation.z);
	ImGui::PopItemWidth();
}

void Camera::SetPosition(const glm::vec3 position)
{
	this->position = position;
	UpdateViewTransformation();
}

void Camera::SetRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
	UpdateViewTransformation();
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
								up);
	viewProjMatrix = projectionMatrix * viewMatrix;

	
	glm::vec3 scale, translation, skew;
	glm::quat qrot;
	glm::vec4 perspective;
	glm::decompose(viewProjMatrix, scale, qrot, translation, skew, perspective);
	rotation = glm::eulerAngles(qrot) * 3.14159f / 180.f;
}
void Camera::UpdateViewTransformationLookAround()
{
	//forward === 
	glm::vec3 direction;
	direction.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	direction.y = sin(glm::radians(rotation.x));
	direction.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward = glm::normalize(direction);

	//right ===
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(worldUp, forward));
	//up ===
	up = glm::cross(forward, right);

	viewMatrix = glm::lookAt(position,
		position + forward,
		up);
	viewProjMatrix = projectionMatrix * viewMatrix;
	
}

NAMESPACE_END
