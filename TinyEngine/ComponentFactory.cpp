#include "ComponentFactory.h"

ComponentFactory & ComponentFactory::getInstance()
{
	static ComponentFactory instance;
	return instance;
}

bool ComponentFactory::Register(std::string key, Builder builder)
{
	return registeredMap.insert(std::make_pair(key, builder)).second;
}

Component * ComponentFactory::Create(std::string key, Object* object)
{
	auto it = registeredMap.find(key);
	if (it == registeredMap.end())
	{
		return nullptr;
	}

	return (it->second)(object);
}

ComponentFactory ::ComponentFactory() {}