#include "FrameBufferBuilder.h"
#include "LightManager.h"
#include "LoadManager.h"
#include "MeshRenderer.h"
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

BloomStage::BloomStage()
{
	FrameBufferBuilder* builder = new FrameBufferBuilder();
	extractFrameBuffer = builder
		->SetWidth(SystemSettings::WINDOW_WIDTH)
		->SetHeight(SystemSettings::WINDOW_HEIGHT)
		->SetSamples(1)
		->AppendColorBuffer(USE_TEXTURE)
		->SetDepthBuffer(USE_RBO)
		->SetStencilBuffer(USE_RBO)
		->CombineDepthAndStencil(true)
		->AllowHDR(true)
		->Build();

	blurFrameBuffer[0] = builder->Build();
	blurFrameBuffer[1] = builder->Build();

	mixFrameBuffer = builder->Build();

	delete builder;

	extractShader = new Shader("Shaders/bloom_extract.vs", "Shaders/bloom_extract.fs");
	extractMaterial = new Material(extractShader);

	blurShader = new Shader("Shaders/bloom_blur.vs", "Shaders/bloom_blur.fs");
	blurMaterial = new Material(blurShader);

	mixShader = new Shader("Shaders/bloom_mix.vs", "Shaders/bloom_mix.fs");
	mixMaterial = new Material(mixShader);

	quad = ObjectBuilder::CreateObject(LoadManager::getInstance().LoadGeometryData("quad.mesh"), extractMaterial, 4000, glm::vec3(0.0f, 0.0f, -0.0f));
}

IFrameBuffer * BloomStage::Render(IFrameBuffer * inputFrameBuffer, bool isFinal)
{
	MeshRenderer* renderer = (MeshRenderer*)(quad->GetComponent(MeshRenderer::MESH_RENDERER_NAME));

	extractFrameBuffer->Bind();
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	extractMaterial->setTexture("screenTexture", (Texture*)(inputFrameBuffer->colorBuffers[0]));
	renderer->SetMaterial(extractMaterial);
	quad->draw(Camera::GetWorldCamera());
	extractFrameBuffer->Unbind();

	IFrameBuffer* lastFrameBuffer = extractFrameBuffer;
	int blurCount = 10;
	int nextIndex = 0;
	for (int i = 0; i < blurCount; i++)
	{
		IFrameBuffer* nextFrameBuffer = blurFrameBuffer[nextIndex];

		nextFrameBuffer->Bind();
		glDisable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		blurMaterial->setTexture("screenTexture", (Texture*)(lastFrameBuffer->colorBuffers[0]));
		blurMaterial->setBool("horizontal", nextIndex == 0);
		renderer->SetMaterial(blurMaterial);
		quad->draw(Camera::GetWorldCamera());
		nextFrameBuffer->Unbind();

		lastFrameBuffer = nextFrameBuffer;
		nextIndex = 1 - nextIndex;
	}

	mixFrameBuffer->Bind();
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	mixMaterial->setTexture("screenTexture", (Texture*)(inputFrameBuffer->colorBuffers[0]));
	mixMaterial->setTexture("bloomTexture", (Texture*)(lastFrameBuffer->colorBuffers[0]));
	renderer->SetMaterial(mixMaterial);
	quad->draw(Camera::GetWorldCamera());
	mixFrameBuffer->Unbind();

	return mixFrameBuffer;
}
