#include "ObjectBuilder.h"

Object * ObjectBuilder::CreateObject(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	return CreateObject(nullptr, nullptr, 1000, position, rotation, scale);
}

Object * ObjectBuilder::CreateObject(Geometry * geometry, Material * material, int renderQueue, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	return getInstance().CreateObjectInternal(geometry, material, renderQueue, position, rotation, scale);
}

ObjectBuilder & ObjectBuilder::getInstance()
{
	static ObjectBuilder instance;
	return instance;
}

Object * ObjectBuilder::CreateObjectInternal(Geometry * geometry, Material * material, int renderQueue, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	Object* object = new Object(nextObjectId++);

	Transform* transform = (Transform*)(object->AddComponent<Transform>());
	transform->setPosition(position);
	transform->setRotation(rotation);
	transform->setScale(scale);

	if (geometry != nullptr)
	{
		MeshFilter* meshFilter = (MeshFilter*)(object->AddComponent<MeshFilter>());
		meshFilter->SetTarget(geometry);
	}

	if (material != nullptr)
	{
		MeshRenderer* meshRenderer = (MeshRenderer*)(object->AddComponent<MeshRenderer>());
		meshRenderer->SetMaterial(material);
		meshRenderer->SetRenderQueue(renderQueue);
	}

	ObjectManager::getInstance().AddObject(object);
	return object;
}

ObjectBuilder::ObjectBuilder() {}