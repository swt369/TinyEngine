#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <string>
#include <vector>
using namespace std;

#include <glm/glm.hpp>

enum GeometryRenderType
{
	NORMAL,
	INSTANCED,
};

class Geometry
{
public:
	virtual void Draw() = 0;
	virtual void RefreshModelMatrices(vector<glm::mat4>* modelMatrices) = 0;
};

#endif // !GEOMETRY_H