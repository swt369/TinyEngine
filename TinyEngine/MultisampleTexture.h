#ifndef MULTISAMPLE_TEXTURE_H
#define MULTISAMPLE_TEXTURE_H

#include <iostream>

#include <glad/glad.h>

#include "SystemSettings.h"
#include "Texture.h"

class MultisampleTexture : public ITexture
{
public:
	MultisampleTexture(int samples);

	void Bind(int unit = 0) override;
private:
	int samples;
};

#endif // !MULTISAMPLE_TEXTURE_H