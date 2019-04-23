//#include "MultisampleFrameBuffer.h"
//
//MultisampleFrameBuffer::MultisampleFrameBuffer(int samples) : samples(samples)
//{
//	glGenFramebuffers(1, &FBO);
//	Bind();
//
//	colorBuffer = new MultisampleTexture(samples);
//	colorBuffer->Bind();
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBuffer->ID, 0);
//
//	glGenRenderbuffers(1, &RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
//	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT);
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);
//
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//	{
//		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
//	}
//
//	Unbind();
//}
//
//void MultisampleFrameBuffer::Bind()
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//}
//
//void MultisampleFrameBuffer::Unbind()
//{
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}
//
//void MultisampleFrameBuffer::Delete()
//{
//	glDeleteFramebuffers(1, &FBO);
//}
//
//void MultisampleFrameBuffer::BindReadOnly()
//{
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
//}
//
//void MultisampleFrameBuffer::UnbindReadOnly()
//{
//	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
//}
//
//void MultisampleFrameBuffer::BindDrawOnly()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
//}
//
//void MultisampleFrameBuffer::UnbindDrawOnly()
//{
//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//}
//
