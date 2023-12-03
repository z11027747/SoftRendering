#ifndef _ALL_DEFINE_H
#define _ALL_DEFINE_H

#include "vector.h"

struct Viewport {
	int x, y;
	int w, h;

	Viewport() {
	}

	Viewport(int x, int y, int w, int h)
		: x(x), y(y), w(w), h(h)
	{
	}
};

//顶点
struct Vertex {
	Vector4<float> position;
	Color color;
	Vector2<float> uv;

	Vertex() {
	}

	Vertex(Vector3<float> p, Color c, Vector2<float> u)
		: position(Vector4<float>(p, 1.0f)), color(c), uv(u)
	{
	}

	Vertex(Vector4<float> p, Color c)
		: position(p), color(c)
	{
	}
};

//贴图
class Texture {

public:
	int w, h, c;
	unsigned char* data;

	Color Tex(const Vector2<float>& uv) const {

		//Point
		// uv:0.0-1.0
		int index = ((int)(uv.x * w) + (int)(uv.y * h) * w) * c;
		Color result(data[index + 0], data[index + 1], data[index + 2]);
		return result;
	}
};

//平底梯形
struct Trapezoid {
	Vertex ld;
	Vertex lt;
	Vertex rd;
	Vertex rt;
};

//扫描线
struct Scanline {
	Vertex left;
	Vertex right;
	float step;
};


#endif 
