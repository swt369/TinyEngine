#include "RenderPipeline.h"

RenderPipeline::RenderPipeline()
{
	renderShadowMapStage = new RenderShadowMapStage();
	renderObjectStage = new RenderObjectStage();
	finalStage = new PostProcessingStage(new Shader("Shaders/postprocessing.vs", "Shaders/postprocessing_nothing.fs"));

	stages.push_back(renderShadowMapStage);
	stages.push_back(renderObjectStage);
	stages.push_back(finalStage);
}

void RenderPipeline::Render()
{
	IFrameBuffer* nextFrameBuffer = nullptr;

	for (int i = 0; i < stages.size(); i++)
	{
		if (!stages[i]->isEnabled)
		{
			continue;
		}

		nextFrameBuffer = stages[i]->Render(nextFrameBuffer, i == stages.size() - 1);
	}
}

void RenderPipeline::SetMSAA(bool enabled)
{
	renderObjectStage->SetMSAA(enabled);
}