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

	bool isPerspective;

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
	float yaw = 0.0f;
	float pitch = 0.0f;

	float fov = 45.0f;
	float aspectRatio = (float)SystemSettings::WINDOW_WIDTH / (float)SystemSettings::WINDOW_HEIGHT;

	float orthoLeft = -10.0f;
	float orthoRight = 10.0f;
	float orthoBottom = -10.0f;
	float orthoUp = 10.0f;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;

	float speed = 5.0f;
	float mouseSensitivity = 0.1f;
	float scrollSensitivity = 0.1f;

	Camera(bool isPerspective = true);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();
	void ProcessKeyboard(Direction dir, float deltaTime);
	void ProcessMouse(float offsetX, float offsetY);
	void ProcessScroll(float offset);
	void UpdateCameraVectors();
	void PrintInfo();
};

#endif // !CAMERA_H
