#pragma once

#include "GL/glew.h"

#include <iostream>

class Texture2D
{
public:
	unsigned int ID;
	unsigned int Width, Height;
	unsigned int InternalFormat;
	unsigned int SourceFormat;

	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Filter_Min;
	unsigned int Filter_Max;

public:
	Texture2D();
	void Generate(unsigned int width, unsigned int height, unsigned char *data);
	void Bind() const;
};