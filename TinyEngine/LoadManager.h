#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshLoader.h"
#include "Object.h"
#include "ObjectBuilder.h"
#include "PointMapLoader.h"
#include "Texture.h"

class LoadManager
{
public:
	static LoadManager& getInstance();

	Geometry* LoadGeometryData(string path);
	vector<Object*> LoadModelData(string path, GeometryRenderType type = NORMAL);
private:
	LoadManager();

	void ProcessNode(string rootPath, vector<Mesh*>* meshes, vector<Material*>* materials, aiNode* aiRoot,const aiScene* aiScene, GeometryRenderType type);
	pair<Mesh*, Material*> ProcessMesh(string rootPath, aiMesh* aiMesh, const aiScene* aiScene, GeometryRenderType type);
	vector<Texture*> LoadTextureFromModel(string rootPath, aiMaterial *aiMaterial, aiTextureType aiTextureType);
};

#endif // !LOAD_MANAGER_H
