#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <string>
#include <iostream>
using namespace std;

#include <glad/glad.h>
#include "stb_image.h"

class Cubemap
{
public:
	Cubemap(string right, string left, string top, string bottom, string back, string front);

	void Bind(int unit);

	unsigned int ID;
private:

};

#endif // !CUBEMAP_H
