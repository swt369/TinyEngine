#ifndef POINT_H
#define POINT_H

#include <glm/glm.hpp>

struct Point
{
	glm::vec3 position;
	glm::vec3 color;

	Point(float* data);
	Point(glm::vec3 position, glm::vec3 color);
	Point(float posX, float posY, float posZ, float r, float g, float b);
};

#endif // !POINT_H