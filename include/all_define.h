#ifndef _ALL_DEFINE_H
#define _ALL_DEFINE_H

#include <iostream>
#include <map>

#include "vector.h"
#include "color.h"

//视口
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

//视锥
struct Frustum {
	float near;
	float far;
	float aspect;
	float fov;
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

	Vertex(Vector4<float> p, Color c, Vector2<float> u)
		: position(p), color(c), uv(u)
	{
	}

	friend Vertex operator/ (const Vertex& m, float v) {
		Vertex result;
		result.position = m.position / v;
		result.color = m.color / v;
		result.uv = m.uv / v;

		return result;
	}

	//透视矫正，按z取反
	Vertex Correction() const {
		float z = position.z;

		Vertex result;
		result.position = position;
		result.position.z = 1 / z;
		result.color = color / z;
		result.uv = uv / z;

		return result;
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
};


#endif 
