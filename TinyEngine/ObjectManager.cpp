#include "MapUtil.h"
#include "ObjectManager.h"

ObjectManager & ObjectManager::getInstance()
{
	static ObjectManager instance;
	return instance;
}

bool ObjectManager::AddObject(Object* object)
{
	if (MapUtil::Contains(&objectMap, object->id))
	{
		return false;
	}

	objectMap.insert(make_pair(object->id, object));
	return true;
}

bool ObjectManager::DeleteObject(Object* object)
{
	if (!MapUtil::Contains(&objectMap, object->id))
	{
		return false;
	}

	objectMap.erase(object->id);
	return true;
}

void ObjectManager::ForEach(Handler handler)
{
	for (auto it = objectMap.begin(); it != objectMap.end(); it++)
	{
		handler(it->second);
	}
}

ObjectManager :: ObjectManager() {}
