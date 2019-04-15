#include "Point.h"

Point::Point(float * data)
{
	position = glm::vec3(data[0], data[1], data[2]);
	color = glm::vec3(data[3], data[4], data[5]);
}

Point::Point(glm::vec3 position, glm::vec3 color)
{
	this->position = position;
	this->color = color;
}

Point::Point(float posX, float posY, float posZ, float r, float g, float b)
{
	position = glm::vec3(posX, posY, posZ);
	color = glm::vec3(r, g, b);
}
