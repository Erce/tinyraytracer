/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/
#pragma once
#include "Vector3.h"
typedef Vector3 Colour3f;
#define COLOUR_RED		Colour3f(1.f, 0.f, 0.f)
#define COLOUR_GREEN	Colour3f(0.f, 1.f, 0.f)
#define COLOUR_BLUE		Colour3f(0.f, 0.f, 1.f)
#define COLOUR_WHITE	Colour3f(1.f, 1.f, 1.f)
#define COLOUR_GREY 	Colour3f(.5f, .5f, .5f)
#define COLOUR_BLACK 	Colour3f(.0f, .0f, .0f)


typedef unsigned int Pixel;

/*
*	converts floating point colour value to pixel colour value
*/
inline void clampColour3fToPixel(const Colour3f& colour, Pixel& pixel )
{
	unsigned int red	= (unsigned int)(colour.r * 256);
	unsigned int green	= (unsigned int)(colour.g * 256);
	unsigned int blue	= (unsigned int)(colour.b * 256);
	if (red > 255) red = 255;
	if (green > 255) green = 255;
	if (blue > 255) blue = 255;
	pixel = (red << 16) | (green << 8) | blue;
}