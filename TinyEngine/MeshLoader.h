#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "Mesh.h"
#include "StringUtil.h"

class MeshLoader
{
public:
	static MeshLoader& getInstance();
	Mesh* LoadMesh(ifstream* in);
private:
	MeshLoader();
};

#endif // !MESH_LOADER_H