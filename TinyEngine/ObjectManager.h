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

	Object* Next();
private:
	map<int, Object*> objectMap;

	int curId = 0;

	ObjectManager();
};

#endif // !OBJECT_MANAGER_H