#include "LightManager.h"
#include "ObjectBuilder.h"
#include "RenderManager.h"
#include "ShadowMapRenderer.h"

const int ShadowMapRenderer::DEFAULT_SHADOW_MAP_WIDTH = 1280;
const int ShadowMapRenderer::DEFAULT_SHADOW_MAP_HEIGHT = 720;

const float ShadowMapRenderer::DEFAULT_ORTHO_LEFT = -20.0f;
const float ShadowMapRenderer::DEFAULT_ORTHO_RIGHT = 20.0f;
const float ShadowMapRenderer::DEFAULT_ORTHO_BOTTOM = -20.0f;
const float ShadowMapRenderer::DEFAULT_ORTHO_UP = 20.0f;
const float ShadowMapRenderer::DEFAULT_NEAR_PLANE = 0.1f;
const float ShadowMapRenderer::DEFAULT_FAR_PLANE = 100.0f;
const float ShadowMapRenderer::DEFAULT_DEPTH = -10.0f;


ShadowMapRenderer::ShadowMapRenderer()
{
	shadowMapShader = new Shader("Shaders/depthMap.vs", "Shaders/depthMap.fs");
	shadowMapFrameBuffer = new FrameBuffer(DEFAULT_SHADOW_MAP_WIDTH, DEFAULT_SHADOW_MAP_HEIGHT, NON, USE_TEXTURE, NON);

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

ShadowMapRenderer & ShadowMapRenderer::getInstance()
{
	static ShadowMapRenderer instance;
	return instance;
}

void ShadowMapRenderer::RenderShadowMap()
{
	Light* mainLight = LightManager::getInstance().getMainLight();
	if (mainLight == nullptr)
	{
		return;
	}
	shadowMapCamera->GetTransform()->CopyTransform(mainLight->GetOwner());

	RenderManager::getInstance().SortRenderQueue();
	shadowMapFrameBuffer->Bind();
	glViewport(0, 0, DEFAULT_SHADOW_MAP_WIDTH, DEFAULT_SHADOW_MAP_HEIGHT);
	glCullFace(GL_FRONT);
	RenderManager::getInstance().RenderObjects(shadowMapCamera, shadowMapShader);
	glCullFace(GL_BACK);
	glViewport(0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT);

	shadowMapFrameBuffer->Unbind();
}

Texture * ShadowMapRenderer::GetShadowMap()
{
	return (Texture*)shadowMapFrameBuffer->depthBuffer;
}
