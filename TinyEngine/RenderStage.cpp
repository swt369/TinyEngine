#include "FrameBufferBuilder.h"
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
	FrameBufferBuilder* builder = new FrameBufferBuilder();
	frameBuffer = builder
		->SetWidth(SystemSettings::WINDOW_WIDTH)
		->SetHeight(SystemSettings::WINDOW_HEIGHT)
		->SetSamples(1)
		->AppendColorBuffer(USE_TEXTURE)
		->SetDepthBuffer(USE_RBO)
		->SetStencilBuffer(USE_RBO)
		->CombineDepthAndStencil(true)
		->AllowHDR(true)
		->Build();

	multisampleFrameBuffer = builder
		->SetWidth(SystemSettings::WINDOW_WIDTH)
		->SetHeight(SystemSettings::WINDOW_HEIGHT)
		->SetSamples(4)
		->AppendColorBuffer(USE_TEXTURE)
		->SetDepthBuffer(USE_RBO)
		->SetStencilBuffer(USE_RBO)
		->CombineDepthAndStencil(true)
		->AllowHDR(true)
		->Build();

	delete builder;
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
	FrameBufferBuilder* builder = new FrameBufferBuilder();
	outputFrameBuffer = builder
		->SetWidth(SystemSettings::WINDOW_WIDTH)
		->SetHeight(SystemSettings::WINDOW_HEIGHT)
		->SetSamples(1)
		->AppendColorBuffer(USE_TEXTURE)
		->SetDepthBuffer(USE_RBO)
		->SetStencilBuffer(USE_RBO)
		->CombineDepthAndStencil(true)
		->AllowHDR(true)
		->Build();
	delete builder;

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
	postProcessingMaterial->setTexture("screenTexture", (Texture*)(inputFrameBuffer->colorBuffers[0]));
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
