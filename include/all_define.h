#ifndef _ALL_DEFINE_H
#define _ALL_DEFINE_H

#include "vector.h"


//平底梯形
struct Trapezoid {
	int top;
	int bottom;
	Vector2 ld;
	Vector2 lt;
	Vector2 rd;
	Vector2 rt;
};

//扫描线
struct Scanline {
	int start;
	int width;
};

#endif 
