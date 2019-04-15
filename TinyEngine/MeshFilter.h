#ifndef MESH_FILTER_H
#define MESH_FILTER_H

#include <vector>

#include "Component.h"
#include "Geometry.h"

class MeshFilter : public Component
{
public:
	const static string MESH_FILTER_NAME;

	MeshFilter(Object* object);

	void SetTarget(Geometry* target);
	Geometry* GetTarget();

	string GetComponentName() override;
private:
	Geometry* target;
};

#endif // !MESH_FILTER_H