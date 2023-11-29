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

		DrawTriangle(Vector2<int>(20, 20), Vector2<int>(40, 40), Vector2<int>(60, 20), Color::red);
		DrawLine(Vector2<int>(-10, 10), Vector2<int>(10, 10), Color::white);
		DrawLine(Vector2<int>(40, 50), Vector2<int>(60, 10), Color::green);
		DrawLine(Vector2<int>(40, 10), Vector2<int>(60, 50), Color::green);
	}

	//绘制直线
	void DrawLine(Vector2<int> start, Vector2<int> end, const Color& color) {

		bool clipResult = cohenSutherlandLineClip(start, end, Vector2<int>(w - 1, h - 1));
		if (clipResult) {
			bresenmanDrawLine(start, end, color);
		}
	}

	//绘制三角形
	void DrawTriangle(Vector2<int> p1, Vector2<int> p2, Vector2<int> p3, const Color& color)
	{
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p1, p3, color);
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
