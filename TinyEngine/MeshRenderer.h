#ifndef MESH_RENDERER_H
#define MESH_RENDERER_H

#include "Camera.h"
#include "Component.h"
#include "Geometry.h"
#include "LightManager.h"
#include "Material.h"
#include "MeshFilter.h"
#include "Transform.h"

enum PolygonMode
{
	FILL,
	Wireframe,
};

class MeshRenderer : public Component
{
public:
	static const string MESH_RENDERER_NAME;

	PolygonMode polygonMode = FILL;

	MeshRenderer(Object* object);

	string GetComponentName() override;

	void SetMaterial(Material* material);
	Material* GetMaterial();
	void SetRenderQueue(int renderQueue);
	int GetRenderQueue();
	void Draw(Camera* camera);
private:
	int renderQueue;
	Material* material;
};

#endif // !MESH_RENDERER_H