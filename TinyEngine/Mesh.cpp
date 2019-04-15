#include "Mesh.h"

Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, GeometryRenderType type) : vertices(vertices), indices(indices), type(type)
{
	SetupMesh();

	if (type == INSTANCED)
	{
		glGenBuffers(1, &instancedArrayVBO);
	}
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	if (type == NORMAL)
	{
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, inctancingAmount);
	}
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_STATIC_DRAW);
	// Specify how OpenGL should interpret data.
	// arg1: specify which vertex attribute to configure, correspond to the location declared in vertex shader.
	// arg2: the size of the vertex attribute, 3 for vec3.
	// arg3: the type of data.
	// arg4: whether we need an auto-normalize.
	// arg5: the space between the data of two consecutive vertex attributes.
	// arg6: the start offset of data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// Vertex array is disabled in default, so we should enable it.
	// arg1: location of the vertex attribute to be enabled.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);

	// Use EBO to provide the sequence information of the vertices to be drawn.
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

	// Unbind VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind VAO.
	glBindVertexArray(0);
	// Unbind EBO.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RefreshModelMatrices(vector<glm::mat4>* modelMatrices)
{
	if (type != INSTANCED)
	{
		return;
	}

	inctancingAmount = modelMatrices->size();

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, instancedArrayVBO);
	glBufferData(GL_ARRAY_BUFFER, modelMatrices->size() * sizeof(glm::mat4), &(*modelMatrices)[0], GL_STATIC_DRAW);
	GLsizei vec4Size = sizeof(glm::vec4);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0 * vec4Size));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}