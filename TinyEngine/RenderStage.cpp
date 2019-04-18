#include "LoadManager.h"
#include "ObjectBuilder.h"
#include "RenderManager.h"
#include "RenderPipeline.h"
#include "RenderStage.h"
#include "ShadowMapRenderer.h"

RenderObjectStage::RenderObjectStage() {}

IFrameBuffer * RenderObjectStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	RenderManager::getInstance().SortRenderQueue();

	if (!isFinal && inputFrameBuffer != nullptr)
	{
		inputFrameBuffer->Bind();
	}
	RenderManager::getInstance().RenderObjects(Camera::GetWorldCamera());
	if (!isFinal && inputFrameBuffer != nullptr)
	{
		inputFrameBuffer->Unbind();
	}

	return inputFrameBuffer;
}

BlitStage::BlitStage()
{
	outputFrameBuffer = new FrameBuffer();
}

IFrameBuffer * BlitStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	inputFrameBuffer->BindReadOnly();
	outputFrameBuffer->BindDrawOnly();
	glBlitFramebuffer(0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, 0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	inputFrameBuffer->UnbindReadOnly();
	outputFrameBuffer->UnbindDrawOnly();

	return outputFrameBuffer;
}

PostProcessingStage::PostProcessingStage(Shader * postProcessingShader) : postProcessingShader(postProcessingShader)
{
	outputFrameBuffer = new FrameBuffer();
	postProcessingMaterial = new Material(postProcessingShader);
	quad = ObjectBuilder::CreateObject(LoadManager::getInstance().LoadGeometryData("quad.mesh"), postProcessingMaterial, 4000, glm::vec3(0.0f, 0.0f, -0.0f));
}

IFrameBuffer * PostProcessingStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	if (!isFinal && inputFrameBuffer != nullptr)
	{
		outputFrameBuffer->Bind();
	}

	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	postProcessingMaterial->setTexture("screenTexture", (Texture*)(inputFrameBuffer->colorBuffer));
	quad->draw(Camera::GetWorldCamera());

	if (!isFinal && inputFrameBuffer != nullptr)
	{
		outputFrameBuffer->Unbind();
	}
	return outputFrameBuffer;
}

RenderShadowMapStage::RenderShadowMapStage()
{
	shadowMapShader = new Shader("Shaders/shadowMap.vs", "Shaders/shadowMap.fs");
	outputFrameBuffer = new FrameBuffer();
	shadowMapMaterial = new Material(shadowMapShader);
	quad = ObjectBuilder::CreateObject(LoadManager::getInstance().LoadGeometryData("quad.mesh"), shadowMapMaterial, 4000, glm::vec3(0.0f, 0.0f, 0.0f));
}

IFrameBuffer * RenderShadowMapStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//shadowMapMaterial->setTexture("shadowMap", ShadowMapRenderer::getInstance().GetShadowMap());
	quad->draw(Camera::GetWorldCamera());

	return outputFrameBuffer;
}