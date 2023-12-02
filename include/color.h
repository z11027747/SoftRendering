#ifndef _COLOR_H
#define _COLOR_H

#include<iostream>

struct Color {

public:

	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color() {
	}

	Color(
		unsigned char red,
		unsigned char green,
		unsigned char blue)
		:r(red), g(green), b(blue)
	{
	}

	void Print(const char* name) const {
		std::cout << name << ": " << "\n";
		std::cout << "r: " << (int)r << ", g: " << (int)g << ", b: " << (int)b << "\n";
	}

	static Color red;
	static Color green;
	static Color blue;
	static Color white;
};

#endif
