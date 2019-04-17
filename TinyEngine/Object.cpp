#include "Object.h"
#include "MeshRenderer.h"

Object::Object(int id) 
{
	this->id = id;
	componentManager = new ComponentManager(this);
}

void Object::draw(Camera * camera)
{
	MeshRenderer* renderer = (MeshRenderer*)GetComponent(MeshRenderer::MESH_RENDERER_NAME);
	if (renderer == nullptr)
	{
		return;
	}

	renderer->Draw(camera);
}

Component * Object::AddComponent(string name)
{
	return componentManager->AddComponent(name);
}

Component * Object::GetComponent(string name)
{
	return componentManager->GetComponent(name);
}

void Object::RemoveComponent(string name)
{
	componentManager->RemoveComponent(name);
}

Transform * Object::GetTransform()
{
	return (Transform*)GetComponent(Transform::TRANSFORM_NAME);
}
