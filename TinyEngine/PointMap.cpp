#include "PointMap.h"

PointMap::PointMap(vector<Point> points)
{
	this->points = points;

	SetupPointMap();
}

void PointMap::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, points.size());
	glBindVertexArray(0);
}

void PointMap::RefreshModelMatrices(vector<glm::mat4>* modelMatrices)
{

}

void PointMap::SetupPointMap()
{
	// Generate a vertex array object.
	// VAO saves the following data:
	// 1. calls to glEnableVertexAttribArray and glDisableVertexAttribArray;
	// 2. vertex attribute configurations via glVertexAttribPointer.
	// 3. VBO associated with the vertex attributes by calls to glVertexAttribPointer.
	// 4. EBO currently bound.
	// arg1: the number of VAO to be generated.
	// arg2: the address of the VAOs
	glGenVertexArrays(1, &VAO);

	// Generate a vertex buffer object.
	// arg1: the number of VBO to be generated.
	// arg2: the address of the VBOs.
	glGenBuffers(1, &VBO);

	// Bind the current VAO.
	// From this point, we should configure the corresponding VBO and attributes.
	// Then we should unbind the VAO for later use.
	glBindVertexArray(VAO);
	// Bind the current VBO.
	// OpenGL allow us to bind several buffers at once as long as they have different buffer type.
	// arg1: the type of the buffer to be bound.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Pass data to the buffer.
	// arg1: the type of the buffer.
	// arg2: the size of the data to be passed.
	// arg3: the data.
	// arg4: GL_STATIC_DRAW for stable data, GL_DYNAMIC_DRAW for changeable data, GL_STREAM_DRAW for frequently changed data.
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), &(points[0]), GL_STATIC_DRAW);
	// Specify how OpenGL should interpret data.
	// arg1: specify which vertex attribute to configure, correspond to the location declared in vertex shader.
	// arg2: the size of the vertex attribute, 3 for vec3.
	// arg3: the type of data.
	// arg4: whether we need an auto-normalize.
	// arg5: the space between the data of two consecutive vertex attributes.
	// arg6: the start offset of data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)0);
	// Vertex array is disabled in default, so we should enable it.
	// arg1: location of the vertex attribute to be enabled.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (void*)offsetof(Point, position));
	glEnableVertexAttribArray(1);

	// Unbind VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO.
	glBindVertexArray(0);
}
