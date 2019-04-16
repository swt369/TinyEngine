#include "Camera.h"

Camera* Camera::GetWorldCamera()
{
	static Camera* camera = new Camera();
	return camera;
}

Camera::Camera(bool isPerspective) : isPerspective(isPerspective)
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return isPerspective ? glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane) : glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, nearPlane, farPlane);
}

void Camera::ProcessKeyboard(Direction dir, float deltaTime)
{
	switch (dir)
	{
		case FORWARD:
			position += speed * deltaTime * forward;
			break;
		case BACKWARD:
			position -= speed * deltaTime * forward;
			break;
		case LEFT:
			position -= speed * deltaTime * right;
			break;
		case RIGHT:
			position += speed * deltaTime * right;
			break;
	}
}

void Camera::ProcessMouse(float offsetX, float offsetY)
{
	yaw += offsetX * mouseSensitivity;
	if (yaw > 360)
	{
		yaw -= 360;
	}
	else if (yaw < -360)
	{
		yaw += 360;
	}

	pitch = glm::clamp(pitch - offsetY * mouseSensitivity, -89.0f, 89.0f);

	UpdateCameraVectors();
}

void Camera::ProcessScroll(float offset)
{
	fov = glm::clamp(fov + offset, 1.0f, 45.0f);
}

void Camera::UpdateCameraVectors()
{
	forward.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	forward.y = sin(glm::radians(pitch));
	forward.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	right = glm::normalize(glm::cross(forward, worldUp));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::PrintInfo()
{
	printf("Camera position: (%f, %f, %f)\n", position.x, position.y, position.z);
	printf("Camera pitch: %f\n", pitch);
	printf("Camera yaw: %f\n", yaw);
}
