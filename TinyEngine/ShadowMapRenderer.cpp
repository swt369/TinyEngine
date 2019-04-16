#include "RenderManager.h"
#include "ShadowMapRenderer.h"

ShadowMapRenderer::ShadowMapRenderer()
{
	shadowMapShader = new Shader("Shaders/depthMap.vs", "Shaders/depthMap.fs");
	shadowMapFrameBuffer = new FrameBuffer(1280, 720, NON, USE_TEXTURE, NON);
}

ShadowMapRenderer & ShadowMapRenderer::getInstance()
{
	static ShadowMapRenderer instance;
	return instance;
}

void ShadowMapRenderer::RenderShadowMap()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;
	shadowMapShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	RenderManager::getInstance().SortRenderQueue();
	shadowMapFrameBuffer->Bind();
	glViewport(0, 0, 1280, 720);
	glCullFace(GL_FRONT);
	RenderManager::getInstance().RenderObjects(shadowMapShader);
	glCullFace(GL_BACK);
	glViewport(0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT);

	shadowMapFrameBuffer->Unbind();
}

Texture * ShadowMapRenderer::GetShadowMap()
{
	return (Texture*)shadowMapFrameBuffer->depthBuffer;
}
