#include "ComponentManager.h"

ComponentManager::ComponentManager(Object * master)
{
	this->master = master;
}

Component * ComponentManager::AddComponent(string name)
{
	return AddComponentCore(ComponentFactory::getInstance().Create(name, master));
}

Component* ComponentManager::AddComponentCore(Component * component)
{
	if (component == nullptr)
	{
		return nullptr;
	}

	auto components = componentMap[component->GetComponentName()];
	if (components == nullptr)
	{
		components = new set<Component*>();
		componentMap[component->GetComponentName()] = components;
	}

	components->insert(component);
	return component;
}

Component * ComponentManager::GetComponent(string name)
{
	return GetComponentCore(name);
}

Component * ComponentManager::GetComponentCore(string name)
{
	if (name == "")
	{
		return nullptr;
	}

	auto components = componentMap[name];
	if (components == nullptr)
	{
		return nullptr;
	}

	return (*(components->begin()));
}

void ComponentManager::RemoveComponent(string name)
{
	RemoveComponentCore(GetComponent(name));
}

void ComponentManager::RemoveComponentCore(Component * component)
{
	if (component == nullptr)
	{
		return;
	}

	auto components = componentMap[component->GetComponentName()];
	if (components == nullptr)
	{
		return;
	}

	components->erase(component);
	if (components->size() <= 0)
	{
		delete components;
	}
}
