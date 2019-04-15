#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
using namespace std;

class Object;
class Component
{
public:
	virtual void Update();
	virtual string GetComponentName() = 0;

	Object* GetOwner();
	template <typename Derived> Component* AddComponent();
	Component* AddComponent(string name);
	Component* GetComponent(string name);
	void RemoveComponent(string name);
protected:
	Component(Object* obj);
private:
	Object* object;
};

#endif // !COMPONENT_H