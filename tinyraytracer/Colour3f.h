#pragma once
#include "Vector3.h"
typedef Vector3 Colour3f;

typedef unsigned int Pixel;

inline void clampColour3fToPixel(const Colour3f& colour, Pixel& pixel )
{
	unsigned int red	= (unsigned int)(colour.x * 256);
	unsigned int green	= (unsigned int)(colour.y * 256);
	unsigned int blue	= (unsigned int)(colour.z * 256);
	if (red > 255) red = 255;
	if (green > 255) green = 255;
	if (blue > 255) blue = 255;
	pixel = (red << 16) | (green << 8) | blue;
}