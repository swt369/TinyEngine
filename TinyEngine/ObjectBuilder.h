#ifndef OBJECT_BUILDER_H
#define OBJECT_BUILDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.h"
#include "Geometry.h"
#include "Material.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Transform.h"

class ObjectBuilder
{
public:
	static Object* CreateObject(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
	static Object* CreateObject(Geometry * geometry = nullptr, Material * material = nullptr, int renderQueue = 1000,
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
private:
	static ObjectBuilder& getInstance();

	int nextObjectId = 0;

	ObjectBuilder();

	Object* CreateObjectInternal(Geometry * geometry, Material * material, int renderQueue, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
};
#endif // !OBJECT_BUILDER_H