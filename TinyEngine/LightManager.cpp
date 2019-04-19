#include "LightManager.h"

LightManager & LightManager::getInstance()
{
	static LightManager instance;
	return instance;
}

void LightManager::registerLight(Light* light)
{
	string key = light->getTypeName();
	if (lightMap.count(key) > 0)
	{
		lightMap[key].insert(light);
	}
	else
	{
		set<Light*> lights;
		lights.insert(light);
		lightMap[key] = lights;
	}
}

void LightManager::registerMainLight(Light * light)
{
	mainLight = light;
	registerLight(light);
}

void LightManager::unregisterLight(Light * light)
{
	string key = light->getTypeName();
	if (lightMap.count(key) > 0)
	{
		lightMap[key].erase(light);
	}
}

void LightManager::writeLightParams(Material * material)
{
	for (auto itForMap = lightMap.begin(); itForMap != lightMap.end(); itForMap++)
	{
		int id = 0;
		for (auto itForSet = itForMap->second.begin(); itForSet != itForMap->second.end(); itForSet++) 
		{
			(*itForSet)->use(material, id++);
		}

		if (id > 0)
		{
			material->setInt((*(itForMap->second.begin()))->getTypeCountName(), id, false);
		}
	}
}

void LightManager::renderShadowMaps()
{
	for (auto itForMap = lightMap.begin(); itForMap != lightMap.end(); itForMap++)
	{
		int id = 0;
		for (auto itForSet = itForMap->second.begin(); itForSet != itForMap->second.end(); itForSet++)
		{
			(*itForSet)->RenderShadowMap();
		}
	}
}

Light * LightManager::getMainLight()
{
	return mainLight;
}

LightManager::LightManager() {}
