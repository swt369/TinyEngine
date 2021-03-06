#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <map>
#include <string>
#include <set>
#include <type_traits>
using namespace std;

#include "Component.h"
#include "ComponentFactory.h"

class Object;
class ComponentManager
{
public:
	ComponentManager(Object* master);

	template <typename Derived> Derived* AddComponent();
	Component* AddComponent(string name);
	Component* GetComponent(string name);
	void RemoveComponent(string name);
private:
	Object* master;
	map<string, set<Component*>* > componentMap;

	Component* AddComponentCore(Component* component);
	Component* GetComponentCore(string name);
	void RemoveComponentCore(Component* component);
};

template<typename Derived>
inline Derived * ComponentManager::AddComponent()
{
	Derived* res = ComponentFactory::getInstance().Create<Derived>(master);
	AddComponentCore((Component*)res);
	return res;
}

#endif // !COMPONENT_MANAGER_H