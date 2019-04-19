#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <iostream>
using namespace std;

#include <glad/glad.h>
#include "stb_image.h"

#include "Texture.h"

class Cubemap : public ITexture
{
public:
	Cubemap(int width, int height);
	Cubemap(string right, string left, string top, string bottom, string back, string front);

	void Bind(int unit) override;
};

#endif // !CUBEMAP_H
