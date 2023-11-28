#ifndef _COLOR_H
#define _COLOR_H

#include <iostream>

struct Color {
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	Color(unsigned char red, unsigned char green, unsigned char blue)
		:r(red), g(green), b(blue)
	{
		a = 255;
	}
};


#endif
