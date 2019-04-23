#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glad/glad.h>

#include "Cubemap.h"
#include "SystemSettings.h"
#include "Texture.h"

enum FrameBufferSampleType
{
	SINGLESAMPLE_F,
	MULTISAMPLE_F,
};

enum BufferSetting
{
	NON,
	USE_TEXTURE,
	USE_CUBEMAP,
	USE_RBO,
};

class IFrameBuffer
{
public:
	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void BindReadOnly() = 0;
	virtual void UnbindReadOnly() = 0;
	virtual void BindDrawOnly() = 0;
	virtual void UnbindDrawOnly() = 0;
	virtual void Delete() = 0;

	ITexture* colorBuffer;
	ITexture* depthBuffer;
	ITexture* stencilBuffer;
	ITexture* depthAndStencilBuffer;
};

class FrameBuffer : public IFrameBuffer
{
public:
	FrameBuffer(FrameBufferSampleType sampleType = SINGLESAMPLE_F, int samples = 1, int width = SystemSettings::WINDOW_WIDTH, int height = SystemSettings::WINDOW_HEIGHT,
		BufferSetting colorBufferSetting = USE_TEXTURE, BufferSetting depthBufferSetting = USE_RBO, BufferSetting stencilBufferSetting = USE_RBO, bool combineDepthAndStencil = true,
		bool allowHDR = false);

	void Bind() override;
	void Unbind() override;
	void BindReadOnly() override;
	void UnbindReadOnly() override;
	void BindDrawOnly() override;
	void UnbindDrawOnly() override;
	void Delete() override;
private:
	int samples;

	unsigned int FBO;

	unsigned int colorRBO;
	unsigned int depthRBO;
	unsigned int stencilRBO;
	unsigned int depthAndStencilRBO;

	void CreateFrameBufferInternal(int width, int height, 
		BufferSetting colorBufferSetting, BufferSetting depthBufferSetting, BufferSetting stencilBufferSetting, bool combineDepthAndStencil, bool allowHDR);
	void CreateAndBindRenderBufferInternal(unsigned int* RBO, int width, int height, int glFormat, int glAttachment);
	void CreateAndBindTextureInternal(ITexture** texture, int glAttachment, int width, int height,
		int glInternalFormat, int glFormat, int glDatatype = GL_UNSIGNED_BYTE, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	void CreateAndBindCubemapInternal(ITexture** texture, int glAttachment, int width, int height);
};

#endif // !FRAME_BUFFER_H
