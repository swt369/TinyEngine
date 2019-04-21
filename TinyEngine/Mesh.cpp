#include "Mesh.h"

Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices, GeometryRenderType type) : vertices(vertices), indices(indices), type(type)
{
	CalcTangents();
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
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);

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

void Mesh::CalcTangents()
{
	int triangleCount = indices.size() / 3;
	glm::vec3* triangleToTangent = new glm::vec3[triangleCount];
	glm::vec3* triangleToBitangent = new glm::vec3[triangleCount];
	map<int, vector<int>> vertexToTriangles;

	for (int i = 0; i < vertices.size(); i++)
	{
		vertexToTriangles.insert(make_pair(i, vector<int>({})));
	}

	for (int i = 0; i < indices.size(); i++)
	{
		vertexToTriangles[indices[i]].push_back(i / 3);
	}

	for (int i = 0; i < triangleCount; i++)
	{
		glm::vec3 pos0 = vertices[indices[i * 3 + 0]].position;
		glm::vec3 pos1 = vertices[indices[i * 3 + 1]].position;
		glm::vec3 pos2 = vertices[indices[i * 3 + 2]].position;

		glm::vec2 uv0 = vertices[indices[i * 3 + 0]].uv;
		glm::vec2 uv1 = vertices[indices[i * 3 + 1]].uv;
		glm::vec2 uv2 = vertices[indices[i * 3 + 2]].uv;

		glm::vec3 edge01 = pos1 - pos0;
		glm::vec3 edge12 = pos2 - pos1;

		glm::vec2 deltaUV01 = uv1 - uv0;
		glm::vec2 deltaUV12 = uv2 - uv1;

		float f = 1.0f / (deltaUV01.x * deltaUV12.y - deltaUV12.x * deltaUV01.y);

		glm::vec3 tangent;
		tangent.x = f * (deltaUV12.y * edge01.x - deltaUV01.y * edge12.x);
		tangent.y = f * (deltaUV12.y * edge01.y - deltaUV01.y * edge12.y);
		tangent.z = f * (deltaUV12.y * edge01.z - deltaUV01.y * edge12.z);
		triangleToTangent[i] = tangent;

		glm::vec3 bitangent;
		bitangent.x = f * (-deltaUV12.x * edge01.x + deltaUV01.x * edge12.x);
		bitangent.y = f * (-deltaUV12.x * edge01.y + deltaUV01.x * edge12.y);
		bitangent.z = f * (-deltaUV12.x * edge01.z + deltaUV01.x * edge12.z);
		triangleToBitangent[i] = bitangent;
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		int count = vertexToTriangles[i].size();
		glm::vec3 tangent(0.0f, 0.0f, 0.0f);
		glm::vec3 bitangent(0.0f, 0.0f, 0.0f);

		if (count > 0)
		{
			for (int j = 0; j < count; j++)
			{
				tangent += triangleToTangent[vertexToTriangles[i][j]];
				bitangent += triangleToBitangent[vertexToTriangles[i][j]];
			}
			tangent /= count;
			bitangent /= count;
		}

		vertices[i].tangent = tangent;
		vertices[i].bitangent = bitangent;
	}
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
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0 * vec4Size));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}