#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include <functional>
#include <map>
#include <vector>
using namespace std;

#include "Object.h"

class ObjectManager
{
public:
	typedef std::function<void(Object*)> Handler;

	static ObjectManager& getInstance();

	bool AddObject(Object* object);
	bool DeleteObject(Object* object);
	void ForEach(Handler handler);
private:
	map<int, Object*> objectMap;

	ObjectManager();
};

#endif // !OBJECT_MANAGER_H