#include "LightManager.h"
#include "LoadManager.h"
#include "ObjectBuilder.h"
#include "RenderManager.h"
#include "RenderPipeline.h"
#include "RenderStage.h"
#include "ShadowMapRenderer.h"
#include "SystemSettings.h"

RenderObjectStage::RenderObjectStage()
{
	frameBuffer = new FrameBuffer(SINGLESAMPLE_F, 1, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, USE_TEXTURE, USE_RBO, USE_RBO, true, true);
	multisampleFrameBuffer = new FrameBuffer(MULTISAMPLE_F, 4, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, USE_TEXTURE, USE_RBO, USE_RBO, true, true);
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

IFrameBuffer * RenderShadowMapStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	LightManager::getInstance().renderShadowMaps();
	return nullptr;
}
