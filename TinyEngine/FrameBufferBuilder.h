#ifndef FRAME_BUFFER_BUILDER_H
#define FRAME_BUFFER_BUILDER_H

#include <vector>
using namespace std;

#include "FrameBuffer.h"

class FrameBufferArgs
{
	friend class FrameBufferBuilder;
private:
	FrameBufferArgs();

	void SetWidth(int width);
	void SetHeight(int height);
	void SetSamples(int samples);
	void AppendColorBuffer(BufferSetting setting);
	void SetDepthBuffer(BufferSetting setting);
	void SetStencilBuffer(BufferSetting setting);
	void CombineDepthAndStencil(bool needCombine);
	void AllowHDR(bool allowHDR);

	int width;
	int height;
	int samples;
	vector<BufferSetting> colorBufferSettings;
	BufferSetting depthBufferSetting;
	BufferSetting stencilBufferSetting;
	bool combineDepthAndStencil;
	bool allowHDR;
};

class FrameBufferBuilder
{
public:
	FrameBufferBuilder();

	FrameBufferBuilder* SetWidth(int width);
	FrameBufferBuilder* SetHeight(int height);
	FrameBufferBuilder* SetSamples(int samples);
	FrameBufferBuilder* AppendColorBuffer(BufferSetting setting);
	FrameBufferBuilder* SetDepthBuffer(BufferSetting setting);
	FrameBufferBuilder* SetStencilBuffer(BufferSetting setting);
	FrameBufferBuilder* CombineDepthAndStencil(bool needCombine);
	FrameBufferBuilder* AllowHDR(bool allowHDR);

	FrameBuffer* Build();
private:
	FrameBufferArgs* args;
};
#endif // !FRAME_BUFFER_BUILDER_H