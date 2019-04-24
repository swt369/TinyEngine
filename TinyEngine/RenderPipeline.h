#ifndef RENDER_PIPELINE_H
#define RENDER_PIPELINE_H

#include <vector>
using namespace std;

#include "FrameBuffer.h"
#include "RenderStage.h"

class RenderPipeline
{
public:
	RenderPipeline();

	void Render();
	void SetMSAA(bool enabled);
private:
	vector<IRenderStage*> stages;

	RenderShadowMapStage* renderShadowMapStage;
	RenderObjectStage* renderObjectStage;
	PostProcessingStage* toneMappingStage;
	PostProcessingStage* postProcessingStage;
};

#endif // !RENDER_PIPELINE_H