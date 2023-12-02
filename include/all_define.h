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

	Vertex() {
	}

	Vertex(Vector3<float> p, Color c)
		: position(Vector4<float>(p, 1.0f)), color(c)
	{
	}

	Vertex(Vector4<float> p, Color c)
		: position(p), color(c)
	{
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
