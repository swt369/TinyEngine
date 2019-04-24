#ifndef RENDER_STAGE_H
#define RENDER_STAGE_H

#include <glad/glad.h>

#include "FrameBuffer.h"
#include "Light.h"
#include "Material.h"
#include "Object.h"
#include "Shader.h"

class RenderPipeline;
class IRenderStage 
{
public:
	bool isEnabled = true;

	virtual IFrameBuffer* Render(IFrameBuffer* inputFrameBuffer, bool isFinal) = 0;
protected:
	RenderPipeline* pipeline;
};

class RenderObjectStage : public IRenderStage
{
public:
	RenderObjectStage();

	void SetMSAA(bool enabled);

	IFrameBuffer* Render(IFrameBuffer* inputFrameBuffer, bool isFinal) override;
private:
	FrameBuffer* frameBuffer;
	FrameBuffer* multisampleFrameBuffer;

	bool isMSAAEnabled = true;
};

class PostProcessingStage : public IRenderStage
{
public:
	PostProcessingStage(Shader* postProcessingShader);

	IFrameBuffer* Render(IFrameBuffer* inputFrameBuffer, bool isFinal) override;
private:
	IFrameBuffer* outputFrameBuffer;
	Shader* postProcessingShader;
	Material* postProcessingMaterial;
	Object* quad;
};

class RenderShadowMapStage : public IRenderStage
{
public:
	IFrameBuffer* Render(IFrameBuffer* inputFrameBuffer, bool isFinal) override;
};

class BloomStage : public IRenderStage
{
public:
	BloomStage();

	IFrameBuffer* Render(IFrameBuffer* inputFrameBuffer, bool isFinal) override;
private:
	IFrameBuffer* extractFrameBuffer;
	Shader* extractShader;
	Material* extractMaterial;

	IFrameBuffer* blurFrameBuffer[2];
	Shader* blurShader;
	Material* blurMaterial;

	IFrameBuffer* mixFrameBuffer;
	Shader* mixShader;
	Material* mixMaterial;

	Object* quad;
};
#endif // !RENDER_STAGE_H