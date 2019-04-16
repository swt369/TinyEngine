#ifndef OBJECT_H
#define OBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include "ComponentManager.h"
#include "Transform.h"

class Object
{
public:
	int id;

	Transform* transform;

	Object(int id);

	void draw(Camera* camera);

	template <typename Derived> Component* AddComponent();
	Component* AddComponent(string name);
	Component* GetComponent(string name);
	void RemoveComponent(string name);
protected:

private:
	ComponentManager* componentManager;
};

template<typename Derived>
inline Component * Object::AddComponent()
{
	return componentManager->AddComponent<Derived>();
}

#endif // !OBJECT_H