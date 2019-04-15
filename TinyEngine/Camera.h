#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SystemSettings.h"

enum Direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	static Camera* GetWorldCamera();

	glm::vec3 worldUp;

	float yaw;
	float pitch;

	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float fov = 45.0f;
	float aspectRatio;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	float speed = 5.0f;
	float mouseSensitivity = 0.1f;
	float scrollSensitivity = 0.1f;

	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float aspectRatio);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void ProcessKeyboard(Direction dir, float deltaTime);
	void ProcessMouse(float offsetX, float offsetY);
	void ProcessScroll(float offset);
	void UpdateCameraVectors();
	void PrintInfo();
};

#endif // !CAMERA_H
