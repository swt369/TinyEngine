#include "Vertex.h"

Vertex::Vertex(float* data)
{
	position = glm::vec3(data[0], data[1], data[2]);
	normal = glm::vec3(data[3], data[4], data[5]);
	uv = glm::vec2(data[6], data[7]);
}

Vertex::Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
{
	this->position = position;
	this->normal = normal;
	this->uv = uv;
}

Vertex::Vertex(float posX, float posY, float posZ, float normalX, float normalY, float normalZ, float u, float v)
{
	position = glm::vec3(posX, posY, posZ);
	normal = glm::vec3(normalX, normalY, normalZ);
	uv = glm::vec2(u, v);
}
