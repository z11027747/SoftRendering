#ifndef _ALL_DEFINE_H
#define _ALL_DEFINE_H

#include "vector.h"

#define PI 3.1415926

struct Mathf {

	static float Rad2Deg(float rad) {
		return (rad * PI) / 180.0f;
	}
};

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

//平底梯形
struct Trapezoid {
	int top;
	int bottom;
	Vector2<int> ld;
	Vector2<int> lt;
	Vector2<int> rd;
	Vector2<int> rt;

	void Print(const char* name) const {
		std::cout << name << ": " << "\n";
		std::cout << "top: " << top << ", bottom: " << bottom << "\n";
		ld.Print("ld");
		lt.Print("lt");
		rd.Print("rd");
		rt.Print("rt");
	}
};

//扫描线
struct Scanline {
	int start;
	int width;
};

//顶点
struct Vertex {
	Vector4<float> position;
	Vector3<char> color;

	Vertex(Vector3<float> p, Vector3<char> c)
		: position(Vector4<float>(p, 1.0f)), color(c)
	{
	}
};

#endif 
