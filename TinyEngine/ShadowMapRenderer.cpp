#include "FrameBufferBuilder.h"
#include "LightManager.h"
#include "ObjectBuilder.h"
#include "RenderManager.h"
#include "ShadowMapRenderer.h"
#include "StringUtil.h"
#include "Transform.h"

const int IShadowMapRenderer::DEFAULT_SHADOW_MAP_WIDTH = 1280;
const int IShadowMapRenderer::DEFAULT_SHADOW_MAP_HEIGHT = 1280;

const float DirectionalShadowMapRenderer::DEFAULT_ORTHO_LEFT = -20.0f;
const float DirectionalShadowMapRenderer::DEFAULT_ORTHO_RIGHT = 20.0f;
const float DirectionalShadowMapRenderer::DEFAULT_ORTHO_BOTTOM = -20.0f;
const float DirectionalShadowMapRenderer::DEFAULT_ORTHO_UP = 20.0f;
const float DirectionalShadowMapRenderer::DEFAULT_NEAR_PLANE = 0.1f;
const float DirectionalShadowMapRenderer::DEFAULT_FAR_PLANE = 100.0f;
const float DirectionalShadowMapRenderer::DEFAULT_DEPTH = -10.0f;

DirectionalShadowMapRenderer::DirectionalShadowMapRenderer(DirectionalLight * light) : light(light)
{
	shadowMapShader = new Shader("Shaders/directionalShadowMap.vs", "Shaders/directionalShadowMap.fs");

	FrameBufferBuilder* builder = new FrameBufferBuilder();
	shadowMapFrameBuffer = builder
		->SetWidth(DEFAULT_SHADOW_MAP_WIDTH)
		->SetHeight(DEFAULT_SHADOW_MAP_HEIGHT)
		->SetSamples(1)
		->SetDepthBuffer(USE_TEXTURE)
		->SetStencilBuffer(NON)
		->Build();
	delete builder;

	shadowMapCameraObj = ObjectBuilder::CreateObject(glm::vec3(0.0f));
	shadowMapCamera = shadowMapCameraObj->AddComponent<Camera>();
	shadowMapCamera->isPerspective = false;
	shadowMapCamera->orthoLeft = DEFAULT_ORTHO_LEFT;
	shadowMapCamera->orthoRight = DEFAULT_ORTHO_RIGHT;
	shadowMapCamera->orthoBottom = DEFAULT_ORTHO_BOTTOM;
	shadowMapCamera->orthoUp = DEFAULT_ORTHO_UP;
	shadowMapCamera->nearPlane = DEFAULT_NEAR_PLANE;
	shadowMapCamera->farPlane = DEFAULT_FAR_PLANE;
	shadowMapCamera->depth = DEFAULT_DEPTH;
}

void DirectionalShadowMapRenderer::RenderShadowMap()
{
	shadowMapCamera->GetTransform()->CopyTransform(light->GetOwner());

	RenderManager::getInstance().SortRenderQueue();
	shadowMapFrameBuffer->Bind();
	glViewport(0, 0, DEFAULT_SHADOW_MAP_WIDTH, DEFAULT_SHADOW_MAP_HEIGHT);
	glCullFace(GL_FRONT);
	RenderManager::getInstance().RenderObjects(shadowMapCamera, shadowMapShader);
	glCullFace(GL_BACK);
	glViewport(0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT);

	shadowMapFrameBuffer->Unbind();
}

ITexture * DirectionalShadowMapRenderer::GetShadowMap()
{
	return shadowMapFrameBuffer->depthBuffer;
}

glm::mat4 DirectionalShadowMapRenderer::GetProjectionMatrix()
{
	return shadowMapCamera->GetProjectionMatrix();
}

const float OmniDirectionalShadowMapRenderer::DEFAULT_NEAR_PLANE = 1.0f;
const float OmniDirectionalShadowMapRenderer::DEFAULT_DEPTH = -10.0f;

const string OmniDirectionalShadowMapRenderer::VIEW_MATRICES_NAME = "viewMatrices";
const string OmniDirectionalShadowMapRenderer::LIGHT_POS_NAME = "lightPos";
const string OmniDirectionalShadowMapRenderer::FAR_PLANE_NAME = "farPlane";
const vector<glm::vec3> OmniDirectionalShadowMapRenderer::viewRotationVectors =
{
	//Positive X
	glm::vec3(0.0f, -90.0f, 0.0f),
	//Negative X
	glm::vec3(0.0f, 90.0f, 0.0f),
	//Positive Y
	glm::vec3(90.0f, 0.0f, 0.0f),
	//Negative Y
	glm::vec3(-90.0f, 0.0f, 0.0f),
	//Positive Z
	glm::vec3(0.0f, 0.0f, 0.0f),
	//Negative Z
	glm::vec3(0.0f, 180.0f, 0.0f),
};

const vector<glm::vec3> OmniDirectionalShadowMapRenderer::ups =
{
	//Positive X
	glm::vec3(0.0f, -1.0f, 0.0f),
	//Negative X
	glm::vec3(0.0f, -1.0f, 0.0f),
	//Positive Y
	glm::vec3(0.0f, 0.0f, 1.0f),
	//Negative Y
	glm::vec3(0.0f, 0.0f, -1.0f),
	//Positive Z
	glm::vec3(0.0f, -1.0f, 0.0f),
	//Negative Z
	glm::vec3(0.0f, -1.0f, 0.0f),
};


OmniDirectionalShadowMapRenderer::OmniDirectionalShadowMapRenderer(PointLight * light) : light(light)
{
	shadowMapShader = new Shader("Shaders/omniDirectionalShadowMap.vs", "Shaders/omniDirectionalShadowMap.fs", "Shaders/omniDirectionalShadowMap.gs");

	FrameBufferBuilder* builder = new FrameBufferBuilder();
	shadowMapFrameBuffer = builder
		->SetWidth(DEFAULT_SHADOW_MAP_WIDTH)
		->SetHeight(DEFAULT_SHADOW_MAP_HEIGHT)
		->SetSamples(1)
		->SetDepthBuffer(USE_CUBEMAP)
		->SetStencilBuffer(NON)
		->Build();
	delete builder;

	shadowMapCameraObj = ObjectBuilder::CreateObject(glm::vec3(0.0f));
	shadowMapCamera = shadowMapCameraObj->AddComponent<Camera>();
	shadowMapCamera->isPerspective = true;
	shadowMapCamera->aspectRatio = (float)DEFAULT_SHADOW_MAP_WIDTH / (float)DEFAULT_SHADOW_MAP_HEIGHT;
	shadowMapCamera->fov = 90.0f;
	shadowMapCamera->nearPlane = DEFAULT_NEAR_PLANE;
	shadowMapCamera->farPlane = light->range;
	shadowMapCamera->depth = DEFAULT_DEPTH;
}

void OmniDirectionalShadowMapRenderer::RenderShadowMap()
{
	Transform* transform = shadowMapCamera->GetTransform();
	transform->setPosition(light->GetTransform()->position);

	shadowMapShader->use();
	for (int i = 0; i < viewRotationVectors.size(); i++)
	{
		transform->setRotation(viewRotationVectors[i]);
		shadowMapShader->setMat4(StringUtil::getKeyInArray(VIEW_MATRICES_NAME, i), shadowMapCamera->GetProjectionMatrix() * transform->getViewMatrix(ups[i]));
	}

	shadowMapShader->setVec3(LIGHT_POS_NAME, transform->position);
	shadowMapShader->setFloat(FAR_PLANE_NAME, shadowMapCamera->farPlane);

	RenderManager::getInstance().SortRenderQueue();
	shadowMapFrameBuffer->Bind();
	glViewport(0, 0, DEFAULT_SHADOW_MAP_WIDTH, DEFAULT_SHADOW_MAP_HEIGHT);
	glCullFace(GL_FRONT);
	RenderManager::getInstance().RenderObjects(shadowMapCamera, shadowMapShader);
	glCullFace(GL_BACK);
	glViewport(0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT);

	shadowMapFrameBuffer->Unbind();
}

ITexture * OmniDirectionalShadowMapRenderer::GetShadowMap()
{
	return shadowMapFrameBuffer->depthBuffer;
}