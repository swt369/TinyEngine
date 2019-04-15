#include "MeshFilter.h"

const string MeshFilter::MESH_FILTER_NAME = "MeshFilter";

MeshFilter::MeshFilter(Object * object) : Component(object) {}

void MeshFilter::SetTarget(Geometry * target)
{
	this->target = target;
}

Geometry * MeshFilter::GetTarget()
{
	return target;
}

string MeshFilter::GetComponentName()
{
	return MESH_FILTER_NAME;
}