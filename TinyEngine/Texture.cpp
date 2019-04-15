#include "Texture.h"

#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

Texture::Texture()
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(std::string imgPath) : Texture(imgPath.c_str())
{

}

Texture::Texture(const char * imgPath)
{
	int width;
	int height;
	int nrChannels;

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannels, 0);
	if (data)
	{
		int colorFlag;
		if (nrChannels == 1)
			colorFlag = GL_RED;
		else if (nrChannels == 3)
			colorFlag = GL_RGB;
		else if (nrChannels == 4)
			colorFlag = GL_RGBA;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Generate a texture
		// arg1: the type of the texture to be generated.
		// arg2: specify the mipmap level for which we want to create a texture for. If we want our image to be the top level, set it to 0.
		// arg3: specify which kind of format we want to store the texture.
		// arg4: and arg5: the width and height of the resulting texture.
		// arg6: 0 for now.
		// arg7 and arg8: format and data type of the source image.
		// arg9: actual image data.
		glTexImage2D(GL_TEXTURE_2D, 0, colorFlag, width, height, 0, colorFlag, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
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
	glBindTexture(GL_TEXTURE_2D, ID);
}