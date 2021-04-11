#pragma once
#include "glm/glm.hpp"

NAMESPACE_BEGAN
class Camera
{
public:
	Camera(float fov = 45.0f, float aspectRatio = 800.0f/ 600.0f , float nearPlan = 0.1f, float farPlane = 100.0f);
	
	void SetOrbit(glm::vec3 lookPosition = {0,0,0});
	
	
	void SetPosition(const glm::vec3 position);
	void SetRotation(glm::vec3 rotation);

	glm::vec3 GetPosition() const { return position; };
	glm::vec3 GetRotation()const { return rotation; };
	inline glm::vec3 GetRight() const  {return right; };
	inline glm::vec3 GetUp() const  {return up; };
	glm::mat4 GetProjectViewMatrix() const;
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
	void UpdateViewTransformOrbit();
	void UpdateViewTransformation();
	void UpdateViewTransformationFreeOrbit();
private:
	bool orbit = true;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjMatrix;

	glm::vec3 position;
	glm::vec3 rotation;

	glm::vec3 forward	= {0,0,1};
	glm::vec3 right		= {1,0,0};
	glm::vec3 up		= {0,1,0};

	glm::vec3 lookPosition;

};
NAMESPACE_END