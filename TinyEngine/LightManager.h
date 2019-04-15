#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <map>
#include <set>
#include <string>
using namespace std;

#include "Light.h"
#include "Shader.h"

class LightManager
{
public:
	static LightManager& getInstance();
	LightManager(LightManager const&) = delete;
	void operator=(LightManager const&) = delete;

	void registerLight(Light* light);
	void unregisterLight(Light* light);
	void writeLightParams(Shader* shader);
private:
	LightManager();

	map<string, set<Light*> > lightMap;
};
#endif // !LIGHT_MANAGER_H