#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
using namespace std;

#include <glad/glad.h>

#include "SystemSettings.h"

enum TextureSampleType 
{
	SINGLESAMPLE_T,
	MULTISAMPLE_T,
};

class ITexture
{
public:
	unsigned int ID;
	virtual void Bind(int unit = 0) = 0;
};

class Texture : public ITexture
{
public:
	Texture(TextureSampleType sampleType = SINGLESAMPLE_T, int samples = 1, int width = SystemSettings::WINDOW_WIDTH, int height = SystemSettings::WINDOW_HEIGHT,
		int internalFormat = GL_RGB, int format = GL_RGB, int dataType = GL_UNSIGNED_BYTE, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	Texture(std::string imgPath, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	Texture(const char* imgPath, int wrapMethodX = GL_REPEAT, int wrapMethodY = GL_REPEAT);
	void Bind(int unit = 0) override;
private:
	int samples;

	void CreateTextureInternal(int samples, unsigned char* data, int width, int height, int glInternalFormat, int glFormat, int glDataType, int glWrapMethodX, int glWrapMethodY);
	void CreateSingleSampleTextureInternal(unsigned char* data, int width, int height, int glInternalFormat, int glFormat, int glDataType, int glWrapMethodX, int glWrapMethodY);
	void CreateMultisampleTextureInternal(int samples, int width, int height, int glInternalFormat);
	unsigned char* LoadTexture(const char* imgPath, int* width, int* height, int* glFormat);
};

#endif // !TEXUTRE_H
