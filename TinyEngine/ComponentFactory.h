#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

#include <functional>
#include <map>
#include <string>

#include "Component.h"

class Object;
class ComponentFactory 
{
public:
	typedef std::function<Component*(Object*)> Builder;

	static ComponentFactory& getInstance();

	bool Register(std::string key, Builder builder);
	template <typename Derived> Component* Create(Object* object);
	Component* Create(std::string key, Object* object);

	template <typename Derived> Component* ComponentBuilder(Object* object) { return new Derived(object); }
private:
	ComponentFactory();

	std::map<string, Builder> registeredMap;
};

template<typename Derived>
inline Component * ComponentFactory::Create(Object* object)
{
	return (Component*)(new Derived(object));
}
#endif // !COMPONENT_FACTORY_H