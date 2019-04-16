#include "RenderPipeline.h"

RenderPipeline::RenderPipeline()
{
	initialFrameBuffer = new FrameBuffer();
	initialMultisampleFrameBuffer = new MultisampleFrameBuffer(4);

	renderObjectStage = new RenderObjectStage();
	blitStage = new BlitStage();
	finalStage = new PostProcessingStage(new Shader("Shaders/postprocessing.vs", "Shaders/postprocessing_nothing.fs"));

	stages.push_back(renderObjectStage);
	stages.push_back(blitStage);
	stages.push_back(finalStage);

	//stages.push_back(new RenderShadowMapStage());
}

void RenderPipeline::Render()
{
	IFrameBuffer* nextFrameBuffer = nullptr;
	if (isMSAAEnabled)
	{
		nextFrameBuffer = initialMultisampleFrameBuffer;
		blitStage->isEnabled = true;
	}
	else
	{
		nextFrameBuffer = initialFrameBuffer;
		blitStage->isEnabled = false;
	}

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
	if (enabled == isMSAAEnabled)
	{
		return;
	}

	isMSAAEnabled = enabled;
}