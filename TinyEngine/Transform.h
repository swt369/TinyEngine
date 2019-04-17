#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Component.h"

class Transform : public Component
{
public:
	const static string TRANSFORM_NAME;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 localScale;

	Transform(Object* object);

	string GetComponentName() override;

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	glm::vec3 getForward();
	glm::vec3 getUp();
	glm::vec3 getRight();

	glm::mat4 getLocalToClipMatrix(Camera* camera);
	glm::mat4 getModelMatrix();
	glm::mat3 getNormalMatrix();
private:
	glm::mat4 localToWorldMatrix;
	void updateLocalToWorldMatrix();
};

#endif // !TRANSFORM_H
