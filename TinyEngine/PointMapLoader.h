#ifndef POINT_MAP_LOADER_H
#define POINT_MAP_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "Point.h"
#include "PointMap.h"
#include "StringUtil.h"

class PointMapLoader
{
public:
	static PointMapLoader& getInstance();
	PointMap* LoadPointMap(ifstream* in);

private:
	PointMapLoader();
};

#endif // !POINT_MAP_LOADER_H