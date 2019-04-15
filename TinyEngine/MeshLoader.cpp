#include "MeshLoader.h"

MeshLoader & MeshLoader::getInstance()
{
	static MeshLoader instance;
	return instance;
}

Mesh * MeshLoader::LoadMesh(ifstream* in)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	bool readingVertices = true;
	string line;
	while (getline(*in, line))
	{
		int length = line.length();
		if (length == 0)
		{
			continue;
		}

		if (line == "Vertex")
		{
			continue;
		}

		if (line == "Index")
		{
			readingVertices = false;
			continue;
		}

		vector<string> data = StringUtil::split(line, ',');

		if (readingVertices)
		{
			float vertexData[8];
			fill(vertexData, vertexData + 8, 0.0f);
			for (int i = 0; i < (data.size() <= 8 ? data.size() : 8); i++)
			{
				vertexData[i] = stof(data[i], 0);
			}
			vertices.push_back(Vertex(vertexData));
		}
		else
		{
			assert(data.size() == 3);

			for (int i = 0; i < 3; i++)
			{
				indices.push_back(stoi(data[i]));
			}
		}
	}

	return new Mesh(vertices, indices);
}

MeshLoader::MeshLoader() {}
