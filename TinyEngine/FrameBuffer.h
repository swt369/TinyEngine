#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glad/glad.h>

#include "Texture.h"

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
};

class FrameBuffer : public IFrameBuffer
{
public:
	FrameBuffer();

	void Bind() override;
	void Unbind() override;
	void BindReadOnly() override;
	void UnbindReadOnly() override;
	void BindDrawOnly() override;
	void UnbindDrawOnly() override;
	void Delete() override;
private:
	unsigned int FBO;
	unsigned int RBO;
};

#endif // !FRAME_BUFFER_H
