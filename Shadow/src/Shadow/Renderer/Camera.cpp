#include "swpch.h"
#include "Camera.h"
#include "Shadow/Input.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

#include  <imgui.h>
#include "Shadow/Window.h"

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
	glm::vec3 lookToPos = position - lookPosition;
	forward = glm::normalize(lookToPos);
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

	//glm::extractEulerAngleXYZ(viewProjMatrix, rotation.x, rotation.y, rotation.z);
	rotation.x = glm::asin(forward.y);
	rotation.y = glm::acos(glm::cos(rotation.x) * forward.x);

	rotation.x = glm::degrees(rotation.x) - 180;
	rotation.y = glm::degrees(rotation.y);
	
	rotation.z = glm::degrees(rotation.z);
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

void Camera::CameraUpdatedInput()
{
	glm::vec3 cameraPos = position;
	glm::vec3 cameraRotation = rotation;

	std::pair<float, float> mousePos = Input::GetMousePosition();
	glm::vec2 mousePosv = glm::vec2(mousePos.first, mousePos.second);
	static glm::vec2 lastMousePos = mousePosv;
	glm::vec2 offset = mousePosv - lastMousePos;

	float speed = 0.5;
	

	//if (Input::IsMouseButtonPressed(SW_MOUSE_BUTTON_1))
	//{
	//	if (!orbit)
	//	{
	//		orbit = true;
	//		
	//		//lookPosition = position + forward * radiusToLookPoint;
	//	}
	//}
		
	if (Input::IsMouseButtonPressed(SW_MOUSE_BUTTON_2))
		orbit = false;

	if (orbit)
	{
		float radiusToLookPoint = (cameraPos - lookPosition).length();

		if (Input::IsKeyPressed(SW_KEY_A))
		{
			cameraPos += right * speed;
		}
		if (Input::IsKeyPressed(SW_KEY_D))
		{
			cameraPos -= right * speed;
		}
		if (Input::IsKeyPressed(SW_KEY_W))
		{
			cameraPos += forward * speed;
		}
		if (Input::IsKeyPressed(SW_KEY_S))
		{
			cameraPos -= forward * speed;
		}
		glm::vec3 lookToCam = cameraPos - lookPosition;
		cameraPos = lookPosition + glm::normalize(lookToCam) * radiusToLookPoint;
	}
	else
	{
		
		

		if (Input::IsMouseButtonPressed(SW_MOUSE_BUTTON_2))
		{
			
			cameraRotation.x -= offset.y;
			cameraRotation.y += offset.x;

			if (cameraRotation.x > 89.0f)
				cameraRotation.x = 89.0f;
			if (cameraRotation.x < -89.0f)
				cameraRotation.x = -89.0f;

		}
		if (Input::IsKeyPressed(SW_KEY_A))
		{
			cameraPos += right * speed;
		}
		if (Input::IsKeyPressed(SW_KEY_D))
		{
			cameraPos -= right * speed;
		}
		if (Input::IsKeyPressed(SW_KEY_W))
		{
			cameraPos += forward * speed;
		}
		if (Input::IsKeyPressed(SW_KEY_S))
		{
			cameraPos -= forward * speed;
		}
		lastMousePos = mousePosv;
	}

	this->SetRotation(cameraRotation);
	this->SetPosition(cameraPos);

}


NAMESPACE_END
