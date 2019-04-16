#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(int width, int height,
	BufferSetting colorBufferSetting, BufferSetting depthBufferSetting, BufferSetting stencilBufferSetting , bool combineDepthAndStencil)
{
	CreateFrameBufferInternal(width, height, colorBufferSetting, depthBufferSetting, stencilBufferSetting, combineDepthAndStencil);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindReadOnly()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
}

void FrameBuffer::UnbindReadOnly()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void FrameBuffer::BindDrawOnly()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

void FrameBuffer::UnbindDrawOnly()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBuffer::Delete()
{
	glDeleteFramebuffers(1, &FBO);
}

void FrameBuffer::CreateFrameBufferInternal(int width, int height, 
	BufferSetting colorBufferSetting, BufferSetting depthBufferSetting, BufferSetting stencilBufferSetting, bool combineDepthAndStencil)
{
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	if (colorBufferSetting == NON)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	else if (colorBufferSetting == USE_TEXTURE)
	{
		CreateAndBindTextureInternal(&colorBuffer, GL_COLOR_ATTACHMENT0, width, height, GL_RGB, GL_RGB);
	}
	else if (colorBufferSetting == USE_RBO)
	{
		CreateAndBindRenderBufferInternal(&colorRBO, width, height, GL_RGB, GL_COLOR_ATTACHMENT0);
	}

	if (depthBufferSetting == stencilBufferSetting)
	{
		if (depthBufferSetting == USE_TEXTURE)
		{
			CreateAndBindTextureInternal(&depthAndStencilBuffer, GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);
		}
		else if (depthBufferSetting == USE_RBO)
		{
			CreateAndBindRenderBufferInternal(&depthAndStencilRBO, width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
		}
	}
	else
	{
		if (depthBufferSetting == USE_TEXTURE)
		{
			CreateAndBindTextureInternal(&depthBuffer, GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
		}
		else if (depthBufferSetting == USE_RBO)
		{
			CreateAndBindRenderBufferInternal(&depthRBO, width, height, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);
		}

		if (stencilBufferSetting == USE_TEXTURE)
		{
			CreateAndBindTextureInternal(&stencilBuffer, GL_DEPTH_STENCIL_ATTACHMENT, width, height, GL_STENCIL_INDEX, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE);
		}
		else if (stencilBufferSetting == USE_RBO)
		{
			CreateAndBindRenderBufferInternal(&stencilRBO, width, height, GL_STENCIL_INDEX, GL_DEPTH_STENCIL_ATTACHMENT);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::CreateAndBindRenderBufferInternal(unsigned int * RBO, int width, int height, int glFormat, int glAttachment)
{
	glGenRenderbuffers(1, RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, *RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, glFormat, width, width);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachment, GL_RENDERBUFFER, *RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::CreateAndBindTextureInternal(ITexture ** texture, int glAttachment, int width, int height, int glInternalFormat, int glFormat, int glDatatype, int wrapMethodX, int wrapMethodY)
{
	*texture = new Texture(width, height, glInternalFormat, glFormat, glDatatype, wrapMethodX, wrapMethodY);
	(*texture)->Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, GL_TEXTURE_2D, (*texture)->ID, 0);
}