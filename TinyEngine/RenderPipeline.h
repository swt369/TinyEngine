#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include <vector>
using namespace std;

#include "FrameBuffer.h"
#include "MultisampleFrameBuffer.h"
#include "RenderStage.h"

class RenderPipeline
{
public:
	RenderPipeline();

	void Render();
	void SetMSAA(bool enabled);
private:
	vector<IRenderStage*> stages;
	FrameBuffer* initialFrameBuffer;
	MultisampleFrameBuffer* initialMultisampleFrameBuffer;

	RenderShadowMapStage* renderShadowMapStage;
	RenderObjectStage* renderObjectStage;
	BlitStage* blitStage;
	PostProcessingStage* finalStage;

	bool isMSAAEnabled = true;
};

#endif // !RENDER_PIPELINE_H