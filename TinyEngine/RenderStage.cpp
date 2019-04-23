#include "LightManager.h"
#include "LoadManager.h"
#include "ObjectBuilder.h"
#include "RenderManager.h"
#include "RenderPipeline.h"
#include "RenderStage.h"
#include "ShadowMapRenderer.h"

RenderObjectStage::RenderObjectStage()
{
	frameBuffer = new FrameBuffer();
	multisampleFrameBuffer = new MultisampleFrameBuffer(4);
}

void RenderObjectStage::SetMSAA(bool enabled)
{
	this->isMSAAEnabled = enabled;
}

IFrameBuffer * RenderObjectStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	RenderManager::getInstance().SortRenderQueue();

	IFrameBuffer* initialFrameBuffer = nullptr;
	if (isMSAAEnabled)
	{
		initialFrameBuffer = multisampleFrameBuffer;
	}
	else
	{
		initialFrameBuffer = frameBuffer;
	}

	initialFrameBuffer->Bind();
	RenderManager::getInstance().RenderObjects(Camera::GetWorldCamera());
	initialFrameBuffer->Unbind();

	if (isMSAAEnabled)
	{
		multisampleFrameBuffer->BindReadOnly();
		frameBuffer->BindDrawOnly();
		glBlitFramebuffer(0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, 0, 0, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		frameBuffer->UnbindDrawOnly();
		multisampleFrameBuffer->UnbindReadOnly();
	}

	return frameBuffer;
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

VisualizeShadowMapStage::VisualizeShadowMapStage()
{
	shadowMapShader = new Shader("Shaders/shadowMap.vs", "Shaders/shadowMap.fs");
	outputFrameBuffer = new FrameBuffer();
	shadowMapMaterial = new Material(shadowMapShader);
	quad = ObjectBuilder::CreateObject(LoadManager::getInstance().LoadGeometryData("quad.mesh"), shadowMapMaterial, 4000, glm::vec3(0.0f, 0.0f, 0.0f));
}

IFrameBuffer * VisualizeShadowMapStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	quad->draw(Camera::GetWorldCamera());

	return outputFrameBuffer;
}

IFrameBuffer * RenderShadowMapStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	LightManager::getInstance().renderShadowMaps();
	return nullptr;
}
