#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;

	Vertex(float* data);
	Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 uv);
	Vertex(float posX, float posY, float posZ, float normalX, float normalY, float normalZ, float u, float v);
};

#endif // !VERTEX_H
