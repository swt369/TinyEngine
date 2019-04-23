//#include "MultisampleTexture.h"
//
//MultisampleTexture::MultisampleTexture(int samples) : samples(samples)
//{
//	glGenTextures(1, &ID);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
//	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, GL_TRUE);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
//}
//
//void MultisampleTexture::Bind(int unit)
//{
//	if (unit < 0 || unit >= 16)
//	{
//		std::cout << "Texture bind error: unit must be in 0~15";
//		return;
//	}
//
//	// Set the texture unit of this texture.
//	glActiveTexture(GL_TEXTURE0 + unit);
//	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, ID);
//}