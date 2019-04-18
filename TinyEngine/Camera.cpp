#include "Camera.h"
#include "Object.h"

const string Camera::CAMERA_NAME = "Camera";
vector<Camera*> Camera::cameras;

Camera* Camera::GetWorldCamera()
{
	Camera* res = nullptr; 
	for (Camera* camera : cameras)
	{
		if (res == nullptr || camera->depth > res->depth)
		{
			res = camera;
		}
	}

	return res;
}

Camera::Camera(Object* object) : Component(object)
{
	cameras.push_back(this);
}

string Camera::GetComponentName()
{
	return CAMERA_NAME;
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return isPerspective ? glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane) : glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, nearPlane, farPlane);
}