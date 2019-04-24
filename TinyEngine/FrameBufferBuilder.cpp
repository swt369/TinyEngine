#include "FrameBufferBuilder.h"
#include "SystemSettings.h"

FrameBufferBuilder::FrameBufferBuilder()
{
	args = new FrameBufferArgs();
}

FrameBufferBuilder * FrameBufferBuilder::SetWidth(int width)
{
	args->SetWidth(width);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::SetHeight(int height)
{
	args->SetHeight(height);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::SetSamples(int samples)
{
	args->SetSamples(samples);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::AppendColorBuffer(BufferSetting setting)
{
	args->AppendColorBuffer(setting);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::SetDepthBuffer(BufferSetting setting)
{
	args->SetDepthBuffer(setting);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::SetStencilBuffer(BufferSetting setting)
{
	args->SetStencilBuffer(setting);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::CombineDepthAndStencil(bool needCombine)
{
	args->CombineDepthAndStencil(needCombine);
	return this;
}

FrameBufferBuilder * FrameBufferBuilder::AllowHDR(bool allowHDR)
{
	args->AllowHDR(allowHDR);
	return this;
}

FrameBuffer * FrameBufferBuilder::Build()
{
	return new FrameBuffer(args->samples, args->width, args->height,
		args->colorBufferSettings, args->depthBufferSetting, args->stencilBufferSetting, args->combineDepthAndStencil, args->allowHDR);
}

FrameBufferArgs::FrameBufferArgs()
{
	width = SystemSettings::WINDOW_WIDTH;
	height = SystemSettings::WINDOW_HEIGHT;
	samples = 1;
	depthBufferSetting = USE_RBO;
	stencilBufferSetting = USE_RBO;
	combineDepthAndStencil = true;
	allowHDR = true;
}

void FrameBufferArgs::SetWidth(int width)
{
	this->width = width;
}

void FrameBufferArgs::SetHeight(int height)
{
	this->height = height;
}

void FrameBufferArgs::SetSamples(int samples)
{
	this->samples = samples;
}

void FrameBufferArgs::AppendColorBuffer(BufferSetting setting)
{
	if (setting == NON || setting == USE_RBO)
	{
		return;
	}

	colorBufferSettings.push_back(setting);
}

void FrameBufferArgs::SetDepthBuffer(BufferSetting setting)
{
	depthBufferSetting = setting;
}

void FrameBufferArgs::SetStencilBuffer(BufferSetting setting)
{
	stencilBufferSetting = setting;
}

void FrameBufferArgs::CombineDepthAndStencil(bool needCombine)
{
	this->combineDepthAndStencil = needCombine;
}

void FrameBufferArgs::AllowHDR(bool allowHDR)
{
	this->allowHDR = allowHDR;
}
