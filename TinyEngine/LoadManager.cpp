#include "LoadManager.h"

LoadManager & LoadManager::getInstance()
{
	static LoadManager instance;
	return instance;
}

Geometry * LoadManager::LoadGeometryData(string path)
{
	ifstream in;
	in.open(path.c_str());
	assert(in.is_open());

	string firstline;
	Geometry* res = nullptr;
	if (getline(in, firstline))
	{
		if (firstline == "#Mesh")
		{
			res =  MeshLoader::getInstance().LoadMesh(&in);
		}
		else if (firstline == "#PointMap")
		{
			res = PointMapLoader::getInstance().LoadPointMap(&in);
		}
	}

	in.close();
	return res;
}

vector<Object*> LoadManager::LoadModelData(string path, GeometryRenderType type)
{
	vector<Object*> res;

	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
		return res;
	}

	vector<Mesh*> meshes;
	vector<Material*> materials;
	ProcessNode(path.substr(0, path.find_last_of('/')), &meshes, &materials, scene->mRootNode, scene, type);

	for (int i = 0; i < meshes.size(); i++)
	{
		res.push_back(ObjectBuilder::CreateObject(meshes[i], materials[i]));
	}

	return res;
}

void LoadManager::ProcessNode(string rootPath, vector<Mesh*>* meshes, vector<Material*>* materials, aiNode * aiNode, const aiScene * aiScene, GeometryRenderType type)
{
	for (int i = 0; i < aiNode->mNumMeshes; i++)
	{
		aiMesh *aiMesh = aiScene->mMeshes[aiNode->mMeshes[i]];
		pair<Mesh*, Material*> pair = ProcessMesh(rootPath, aiMesh, aiScene, type);
		meshes->push_back(pair.first);
		materials->push_back(pair.second);
	}

	for (int i = 0; i < aiNode->mNumChildren; i++)
	{
		ProcessNode(rootPath, meshes, materials, aiNode->mChildren[i], aiScene, type);
	}
}

pair<Mesh*, Material*> LoadManager::ProcessMesh(string rootPath, aiMesh * aiMesh, const aiScene * aiScene, GeometryRenderType type)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	for (int i = 0; i < aiMesh->mNumVertices; i++)
	{
		glm::vec3 position;
		position.x = aiMesh->mVertices[i].x;
		position.y = aiMesh->mVertices[i].y;
		position.z = aiMesh->mVertices[i].z;

		glm::vec3 normal;
		normal.x = aiMesh->mNormals[i].x;
		normal.y = aiMesh->mNormals[i].y;
		normal.z = aiMesh->mNormals[i].z;

		glm::vec2 uv;
		if (aiMesh->mTextureCoords[0])
		{
			uv.x = aiMesh->mTextureCoords[0][i].x;
			uv.y = aiMesh->mTextureCoords[0][i].y;
		}
		else
		{
			uv.x = 0;
			uv.y = 0;
		}

		vertices.push_back(Vertex(position, normal, uv));
	}

	for (int i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Material* material = new Material();
	if (aiMesh->mMaterialIndex >= 0)
	{
		aiMaterial *aiMaterial = aiScene->mMaterials[aiMesh->mMaterialIndex];

		int index = 0;
		for (auto diffuseTexture : LoadTextureFromModel(rootPath, aiMaterial, aiTextureType_DIFFUSE))
		{
			material->setDiffuseTexture(index++, diffuseTexture);
		}

		index = 0;
		for (auto specularTexture : LoadTextureFromModel(rootPath, aiMaterial, aiTextureType_SPECULAR))
		{
			material->setSpecularTexture(index++, specularTexture);
		}
	}

	return make_pair(new Mesh(vertices, indices, type), material);
}

vector<Texture*> LoadManager::LoadTextureFromModel(string rootPath, aiMaterial * aiMaterial, aiTextureType aiTextureType)
{
	vector<Texture*> res;
	for (int i = 0; i < aiMaterial->GetTextureCount(aiTextureType); i++)
	{
		aiString path;
		aiMaterial->GetTexture(aiTextureType, i, &path);
		res.push_back(new Texture(rootPath + '/' + string(path.C_Str())));
	}

	return res;
}

LoadManager::LoadManager() {}