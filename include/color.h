#ifndef _COLOR_H
#define _COLOR_H

struct Color {

public:

	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color(
		unsigned char red,
		unsigned char green,
		unsigned char blue)
		:r(red), g(green), b(blue)
	{
	}

	static Color red;
	static Color green;
	static Color blue;
	static Color white;
};

#endif
