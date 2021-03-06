#include "Component.h"
#include "Object.h"
#include "Transform.h"

Component::Component(Object* object)
{
	this->object = object;
}

void Component::Update() {}

Object * Component::GetOwner()
{
	return object;
}

Transform * Component::GetTransform()
{
	return GetOwner()->GetTransform();
}

template<typename Derived>
inline Component * Component::AddComponent()
{
	return object->AddComponent<Derived>();
}

Component * Component::AddComponent(string name)
{
	return object->AddComponent(name);
}

Component * Component::GetComponent(string name)
{
	return object->GetComponent(name);
}

void Component::RemoveComponent(string name)
{
	object->RemoveComponent(name);
}
