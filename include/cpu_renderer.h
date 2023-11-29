#ifndef _CPU_RENDERER_H
#define _CPU_RENDERER_H

#include <iostream>

#include "color.h"
#include "vector2.h"

class CPURenderer {

public:
	unsigned int w;
	unsigned int h;
	unsigned int channels;

	//颜色值
	unsigned char* colorAttachment;

	CPURenderer(unsigned int width, unsigned int height)
		: w(width), h(height), channels(3)
	{
		colorAttachment = new unsigned char[width * height * channels];
		for (int i = 0; i < width * height * channels; i++)
		{
			colorAttachment[i] = 0;
		}
	}

	~CPURenderer() {
		delete colorAttachment;
	}

	void Render() {

		DrawLine(Vector2<int>(-20, -20), Vector2<int>(40, 40), Color::red);
		DrawLine(Vector2<int>(40, 40), Vector2<int>(60, 20), Color::red);
		DrawLine(Vector2<int>(20, 20), Vector2<int>(600, 20), Color::red);

		DrawLine(Vector2<int>(-10, 10), Vector2<int>(10, 10), Color::white);

		DrawLine(Vector2<int>(40, 50), Vector2<int>(60, 10), Color::green);
		DrawLine(Vector2<int>(40, 10), Vector2<int>(60, 50), Color::green);
	}

	void DrawLine(Vector2<int> start, Vector2<int> end, const Color& color) {

		bool clipResult = cohenSutherlandLineClip(start, end, Vector2<int>(w - 1, h - 1));
		if (clipResult) {
			bresenmanDrawLine(start, end, color);
		}
	}

private:

	//设置像素颜色
	void setColor(int x, int y, const Color& color) const;

	//bresenman直线绘制
	void bresenmanDrawLine(const Vector2<int>& start, const Vector2<int>& end, const Color& color) const;

	//cohenSutherland直线裁剪
	bool cohenSutherlandLineClip(Vector2<int>& v0, Vector2<int>& v1, const Vector2<int>& max);
	int cohenSutherlandLineComputeOutCode(const Vector2<int>& v, const Vector2<int>& max) const;

};

#endif 
