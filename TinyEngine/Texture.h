#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
using namespace std;

#include <glad/glad.h>

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
	Texture(int width = SystemSettings::WINDOW_WIDTH, int height = SystemSettings::WINDOW_HEIGHT,
		int internalFormat = GL_RGB, int format = GL_RGB, int dataType = GL_UNSIGNED_BYTE, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	Texture(unsigned char* data = nullptr, int width = SystemSettings::WINDOW_WIDTH, int height = SystemSettings::WINDOW_HEIGHT, 
		int internalFormat = GL_RGB, int format = GL_RGB, int dataType = GL_UNSIGNED_BYTE, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	Texture(std::string imgPath, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	Texture(const char* imgPath, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	void Bind(int unit = 0) override;
private:
	void CreateTextureInternal(unsigned char* data, int width, int height, int glInternalFormat, int glFormat, int glDataType, int glWrapMethodX, int glWrapMethodY);
	unsigned char* LoadTexture(const char* imgPath, int* width, int* height, int* glFormat);
};

#endif // !TEXUTRE_H
