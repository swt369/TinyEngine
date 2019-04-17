#include "Camera.h"
#include "ObjectController.h"
#include "Transform.h"

ObjectController & ObjectController::getInstance()
{
	static ObjectController instance;
	return instance;
}

void ObjectController::SetTarget(Object * target)
{
	this->target = target;
}

void ObjectController::ProcessKeyboard(Direction dir, float deltaTime)
{
	if (target == nullptr)
	{
		return;
	}

	Transform* transform = target->GetTransform();
	glm::vec3 position = transform->position;
	switch (dir)
	{
		case FORWARD:
			position += speed * deltaTime * transform->getForward();
			break;
		case BACKWARD:
			position -= speed * deltaTime * transform->getForward();
			break;
		case LEFT:
			position += speed * deltaTime * transform->getRight();
			break;
		case RIGHT:
			position -= speed * deltaTime * transform->getRight();
			break;
	}
	transform->setPosition(position);
}

void ObjectController::ProcessMouse(float offsetX, float offsetY)
{
	if (target == nullptr)
	{
		return;
	}

	Transform* transform = target->GetTransform();
	glm::vec3 rotation = transform->rotation;

	float yaw = rotation.y;
	yaw += offsetX * mouseSensitivity;
	if (yaw > 360)
	{
		yaw -= 360;
	}
	else if (yaw < -360)
	{
		yaw += 360;
	}
	rotation.y = yaw;

	float pitch = rotation.x;
	pitch = glm::clamp(pitch - offsetY * mouseSensitivity, -89.0f, 89.0f);
	rotation.x = pitch;

	transform->setRotation(rotation);
}

void ObjectController::ProcessScroll(float offset)
{
	if (target == nullptr)
	{
		return;
	}

	Camera* camera = (Camera*)target->GetComponent(Camera::CAMERA_NAME);
	if (camera == nullptr)
	{
		return;
	}

	camera->fov = glm::clamp(camera->fov + offset, 1.0f, 45.0f);
}

ObjectController::ObjectController() {}
