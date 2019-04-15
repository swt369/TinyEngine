#include "MeshRenderer.h"
#include "RenderManager.h"

const string MeshRenderer::MESH_RENDERER_NAME = "MeshRenderer";

MeshRenderer::MeshRenderer(Object * object) : Component(object)
{
	renderQueue = -1;
}

void MeshRenderer::SetMaterial(Material * material)
{
	this->material = material;
}

Material * MeshRenderer::GetMaterial()
{
	return material;
}

void MeshRenderer::SetRenderQueue(int renderQueue)
{
	RenderManager::getInstance().RemoveRenderer(this);
	this->renderQueue = renderQueue;
	RenderManager::getInstance().AddRenderer(this);
}

int MeshRenderer::GetRenderQueue()
{
	return renderQueue;
}

string MeshRenderer::GetComponentName()
{
	return MESH_RENDERER_NAME;
}

void MeshRenderer::Draw(Camera * camera)
{
	Transform* transform = (Transform*)GetComponent(Transform::TRANSFORM_NAME);
	MeshFilter* meshFilter = (MeshFilter*)GetComponent(MeshFilter::MESH_FILTER_NAME);
	Geometry* target = meshFilter->GetTarget();

	material->Use();
	material->shader->setMat4(Shader::LOCAL_TO_CLIP_MATRIX_NAME, transform->getLocalToClipMatrix(camera));
	material->shader->setMat4(Shader::PROJECTION_MATRIX_NAME, camera->GetProjectionMatrix());
	material->shader->setMat4(Shader::VIEW_MATRIX_NAME, camera->GetViewMatrix());
	material->shader->setMat4(Shader::MODEL_MATRIX_NAME, transform->getModelMatrix());
	material->shader->setMat3(Shader::NORMAL_MATRIX_NAME, transform->getNormalMatrix());
	material->shader->setVec3(Shader::VIEW_POS, camera->position);
	LightManager::getInstance().writeLightParams(material->shader);

	switch (polygonMode)
	{
		case FILL:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case Wireframe:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
	}
	target->Draw();
}
