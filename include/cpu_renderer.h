#ifndef _CPU_RENDERER_H
#define _CPU_RENDERER_H

#include "color.h"

class CPURenderer {

public:
	unsigned int w;
	unsigned int h;

	unsigned char* colorAttachment;

	CPURenderer(unsigned int width, unsigned int height)
		: w(width), h(height)
	{
		colorAttachment = new unsigned char[width * height * 4];
	}

	void Render() {
		for (int i = 0; i < 400; i++)
		{
			SetColor(i, i, Color(255, 0, 0));
		}
	}

	void SetColor(int x, int y, const Color& color) const
	{
		int index = (x + y * w) * 4;
		for (int i = 0; i < 4; i++)
		{
			colorAttachment[index + 0] = color.r;
			colorAttachment[index + 1] = color.g;
			colorAttachment[index + 2] = color.b;
			colorAttachment[index + 3] = color.a;
		}
	}

};

#endif 
