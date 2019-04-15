#ifndef POINT_MAP_H
#define POINT_MAP_H

#include <string>
#include <vector>
using namespace std;

#include <glad/glad.h>

#include "Geometry.h"
#include "Point.h"

class PointMap : public Geometry
{
public:
	PointMap(vector<Point> points);

	void Draw() override;
	void RefreshModelMatrices(vector<glm::mat4>* modelMatrices) override;
private:
	vector<Point> points;

	unsigned int VAO;
	unsigned int VBO;

	void SetupPointMap();
};

#endif // !POINT_MAP_H