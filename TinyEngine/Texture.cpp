#include "Texture.h"

#include <iostream>
#include "stb_image.h"

Texture::Texture(TextureSampleType sampleType, int samples, int width, int height, int internalFormat, int format, int dataType, int wrapMethodX, int wrapMethodY)
{
	CreateTextureInternal(samples, nullptr, width, height, internalFormat, format, dataType, wrapMethodX, wrapMethodY);
}

Texture::Texture(std::string imgPath, int wrapMethodX, int wrapMethodY) : Texture(imgPath.c_str(), wrapMethodX, wrapMethodY) {}

Texture::Texture(const char * imgPath, int wrapMethodX, int wrapMethodY)
{
	int width, height;
	int glFormat;
	unsigned char* data = LoadTexture(imgPath, &width, &height, &glFormat);

	CreateTextureInternal(1, data, width, height, glFormat, glFormat, GL_UNSIGNED_BYTE, wrapMethodX, wrapMethodY);
}

void Texture::Bind(int unit)
{
	if (unit < 0 || unit >= 16)
	{
		std::cout << "Texture bind error: unit must be in 0~15";
		return;
	}

	// Set the texture unit of this texture.
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(samples == 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE, ID);
}

void Texture::CreateTextureInternal(int samples, unsigned char * data, int width, int height, int glInternalFormat, int glFormat, int glDataType, int glWrapMethodX, int glWrapMethodY)
{
	glGenTextures(1, &ID);

	this->samples = samples;
	if (samples == 1)
	{
		CreateSingleSampleTextureInternal(data, width, height, glInternalFormat, glFormat, glDataType, glWrapMethodX, glWrapMethodY);
	}
	else
	{
		CreateMultisampleTextureInternal(samples, width, height, glInternalFormat);
	}

	if (data)
	{
		stbi_image_free(data);
	}
}

void Texture::CreateSingleSampleTextureInternal(unsigned char * data, int width, int height, int glInternalFormat, int glFormat, int glDataType, int glWrapMethodX, int glWrapMethodY)
{
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glFormat, glDataType, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrapMethodX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrapMethodY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::CreateMultisampleTextureInternal(int samples, int width, int height, int glInternalFormat)
{
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, glInternalFormat, width, height, GL_TRUE);
}

unsigned char * Texture::LoadTexture(const char * imgPath, int * width, int * height, int* glFormat)
{
	stbi_set_flip_vertically_on_load(true);

	int nrChannels;
	unsigned char* data = stbi_load(imgPath, width, height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 1)
			*glFormat = GL_RED;
		else if (nrChannels == 3)
			*glFormat = GL_RGB;
		else if (nrChannels == 4)
			*glFormat = GL_RGBA;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		*glFormat = GL_RED;
	}

	return data;
}