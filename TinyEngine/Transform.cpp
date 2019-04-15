#include "Transform.h"

const string Transform::TRANSFORM_NAME = "Transform";

Transform::Transform(Object* object) : Component(object)
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	this->localScale = glm::vec3(1.0f, 1.0f, 1.0f);

	updateLocalToWorldMatrix();
}

string Transform::GetComponentName()
{
	return TRANSFORM_NAME;
}

void Transform::setPosition(glm::vec3 position)
{
	if (this->position == position)
	{
		return;
	}

	this->position = position;
	updateLocalToWorldMatrix();
}

void Transform::setRotation(glm::vec3 rotation)
{
	if (this->rotation == rotation)
	{
		return;
	}

	this->rotation = rotation;
	updateLocalToWorldMatrix();
}

void Transform::setScale(glm::vec3 scale)
{
	if (this->localScale == scale)
	{
		return;
	}

	this->localScale = scale;
	updateLocalToWorldMatrix();
}

glm::mat4 Transform::getLocalToClipMatrix(Camera* camera)
{
	return camera->GetProjectionMatrix() * camera->GetViewMatrix() * getModelMatrix();
}

glm::mat4 Transform::getModelMatrix()
{
	return localToWorldMatrix;
}

glm::mat3 Transform::getNormalMatrix()
{
	return glm::transpose(glm::inverse(glm::mat3(getModelMatrix())));
}

void Transform::updateLocalToWorldMatrix()
{
	localToWorldMatrix = glm::mat4(1.0f);
	localToWorldMatrix = glm::translate(localToWorldMatrix, position);
	localToWorldMatrix = glm::rotate(localToWorldMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	localToWorldMatrix = glm::rotate(localToWorldMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	localToWorldMatrix = glm::rotate(localToWorldMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	localToWorldMatrix = glm::scale(localToWorldMatrix, localScale);
}