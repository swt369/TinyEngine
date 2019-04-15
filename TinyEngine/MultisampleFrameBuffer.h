#ifndef MULTISAMPLE_FRAME_BUFFER_H
#define MULTISAMPLE_FRAME_BUFFER_H

#include <iostream>

#include "FrameBuffer.h"
#include "MultisampleTexture.h"

class MultisampleFrameBuffer : public IFrameBuffer
{
public:
	MultisampleFrameBuffer(int samples);

	virtual void Bind() override;
	virtual void Unbind() override;
	virtual void Delete() override;
	virtual void BindReadOnly() override;
	virtual void UnbindReadOnly() override;
	virtual void BindDrawOnly() override;
	virtual void UnbindDrawOnly() override;
private:
	unsigned int FBO;
	unsigned int RBO;

	int samples;
};

#endif // !MULTISAMPLE_FRAME_BUFFER_H