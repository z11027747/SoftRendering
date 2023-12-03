#ifndef _COLOR_H
#define _COLOR_H

#include<iostream>

struct Color {

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
		std::cout << name << ": ";
		std::cout << "r: " << (int)r << ", g: " << (int)g << ", b: " << (int)b << "\n";
	}

	friend Color operator/ (const Color& l, float v) {

		Color result;
		result.r = (unsigned char)((float)l.r / v);
		result.g = (unsigned char)((float)l.g / v);
		result.b = (unsigned char)((float)l.b / v);

		return result;
	}

	friend Color operator* (const Color& l, const Color& r) {

		Color result;
		result.r = (unsigned char)((l.r * r.r) / 255);
		result.g = (unsigned char)((l.g * r.g) / 255);
		result.b = (unsigned char)((l.b * r.b) / 255);

		return result;
	}

	static Color red;
	static Color green;
	static Color blue;
	static Color white;
};

#endif
