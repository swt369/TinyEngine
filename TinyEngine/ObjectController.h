#ifndef OBJECT_CONTROLLER
#define OBJECT_CONTROLLER

#include "Object.h"

enum Direction
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class ObjectController
{
public:
	static ObjectController& getInstance();

	Object* target;
	void SetTarget(Object* target);

	float speed = 5.0f;
	float mouseSensitivity = 0.1f;
	float scrollSensitivity = 0.1f;
	void ProcessKeyboard(Direction dir, float deltaTime);
	void ProcessMouse(float offsetX, float offsetY);
	void ProcessScroll(float offset);
private:
	ObjectController();
};

#endif // !OBJECT_CONTROLLER