#ifndef _CPU_RENDERER_H
#define _CPU_RENDERER_H

#include "color.h"
#include "vector2.h"

class CPURenderer {

public:
	unsigned int w;
	unsigned int h;
	unsigned int channels;

	unsigned char* colorAttachment;

	CPURenderer(unsigned int width, unsigned int height)
		: w(width), h(height), channels(3)
	{
		colorAttachment = new unsigned char[width * height * channels];
	}

	void Render() {

		DrawLine(Vector2<int>(200, 200), Vector2<int>(400, 400), Color::red);
		DrawLine(Vector2<int>(400, 400), Vector2<int>(600, 200), Color::red);
		DrawLine(Vector2<int>(200, 200), Vector2<int>(600, 200), Color::red);
	}

	//bresenman算法画线
	void DrawLine(const Vector2<int>& start, const Vector2<int>& end, const Color& color) const;

private:
	void setColor(int x, int y, const Color& color) const;
};

#endif 
