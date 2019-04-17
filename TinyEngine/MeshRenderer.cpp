#include "MeshRenderer.h"
#include "RenderManager.h"
#include "ShadowMapRenderer.h"

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

void MeshRenderer::Draw(Camera * camera, Shader* globalShader)
{
	MeshFilter* meshFilter = (MeshFilter*)GetComponent(MeshFilter::MESH_FILTER_NAME);
	if (meshFilter == nullptr)
	{
		return;
	}

	Geometry* target = meshFilter->GetTarget();
	if (target == nullptr)
	{
		return;
	}

	Transform* transform = GetTransform();
	Shader* shaderToBeUsed = globalShader != nullptr ? globalShader : material->shader;

	float near_plane = 1.0f, far_plane = 20.0f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::vec3 lightPos(-8.0f, 16.0f, -4.0f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	material->setTexture("shadowMap", ShadowMapRenderer::getInstance().GetShadowMap());
	material->Use(globalShader);
	shaderToBeUsed->setMat4(Shader::LOCAL_TO_CLIP_MATRIX_NAME, transform->getLocalToClipMatrix(camera));
	shaderToBeUsed->setMat4(Shader::PROJECTION_MATRIX_NAME, camera->GetProjectionMatrix());
	shaderToBeUsed->setMat4(Shader::VIEW_MATRIX_NAME, camera->GetViewMatrix());
	shaderToBeUsed->setMat4(Shader::MODEL_MATRIX_NAME, transform->getModelMatrix());
	shaderToBeUsed->setMat3(Shader::NORMAL_MATRIX_NAME, transform->getNormalMatrix());
	shaderToBeUsed->setVec3(Shader::VIEW_POS, camera->GetTransform()->position);
	LightManager::getInstance().writeLightParams(shaderToBeUsed);
	shaderToBeUsed->setMat4("lightSpaceMatrix", lightSpaceMatrix);

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
