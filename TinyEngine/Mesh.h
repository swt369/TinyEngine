#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>
#include <string>
using namespace std;

#include "Geometry.h"
#include "Vertex.h"

class Mesh : public Geometry
{
public:
	Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, GeometryRenderType type = NORMAL);

	void Draw() override;
	void RefreshModelMatrices(vector<glm::mat4>* modelMatrices) override;
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	unsigned int instancedArrayVBO;

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	GeometryRenderType type;
	int inctancingAmount;

	void SetupMesh();
	void CalcTangents();
};

#endif // !MESH_H
