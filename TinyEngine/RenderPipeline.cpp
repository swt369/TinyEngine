#include "RenderPipeline.h"

RenderPipeline::RenderPipeline()
{
	initialFrameBuffer = new FrameBuffer();
	initialMultisampleFrameBuffer = new MultisampleFrameBuffer(4);

	renderObjectStage = new RenderObjectStage();
	stages.push_back(renderObjectStage);
	blitStage = new BlitStage();
	stages.push_back(blitStage);
	postProcessingStage = new PostProcessingStage(new Shader("postprocessing.vs", "postprocessing_nothing.fs"));
	stages.push_back(postProcessingStage);
}

void RenderPipeline::Render()
{
	IFrameBuffer* nextFrameBuffer;
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