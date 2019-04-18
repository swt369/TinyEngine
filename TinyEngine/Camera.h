#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
using namespace std;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include "SystemSettings.h"

class Camera : public Component
{
public:
	const static string CAMERA_NAME;
	static vector<Camera*> cameras;

	static Camera* GetWorldCamera();

	bool isPerspective = true;

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float fov = 45.0f;
	float aspectRatio = (float)SystemSettings::WINDOW_WIDTH / (float)SystemSettings::WINDOW_HEIGHT;

	float orthoLeft = -10.0f;
	float orthoRight = 10.0f;
	float orthoBottom = -10.0f;
	float orthoUp = 10.0f;

	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	
	float depth = 10.0f;

	Camera(Object* object);

	string GetComponentName() override;

	glm::mat4 GetProjectionMatrix();
};
#endif // !CAMERA_H