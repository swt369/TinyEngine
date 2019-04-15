#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include "SystemSettings.h"

class ITexture
{
public:
	unsigned int ID;
	virtual void Bind(int unit = 0) = 0;
};

class Texture : public ITexture
{
public:
	Texture();
	Texture(std::string imgPath);
	Texture(const char* imgPath);
	void Bind(int unit = 0) override;
};

#endif // !TEXUTRE_H
