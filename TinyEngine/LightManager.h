#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include <map>
#include <set>
#include <string>
using namespace std;

#include "Light.h"
#include "Material.h"
#include "Shader.h"

class LightManager
{
public:
	static LightManager& getInstance();
	LightManager(LightManager const&) = delete;
	void operator=(LightManager const&) = delete;

	void registerLight(Light* light);
	void registerMainLight(Light* light);
	void unregisterLight(Light* light);
	void writeLightParams(Material* material);
	void renderShadowMaps();
	Light* getMainLight();
private:
	LightManager();

	map<string, set<Light*> > lightMap;
	Light* mainLight;
};
#endif // !LIGHT_MANAGER_H
