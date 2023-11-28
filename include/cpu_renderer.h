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

	unsigned char* colorAttachment;

	CPURenderer(unsigned int width, unsigned int height)
		: w(width), h(height), channels(3)
	{
		colorAttachment = new unsigned char[width * height * channels];
	}

	void Render() {

		DrawLine(Vector2<int>(-200, -200), Vector2<int>(400, 400), Color::red);
		DrawLine(Vector2<int>(400, 400), Vector2<int>(600, 200), Color::red);
		DrawLine(Vector2<int>(200, 200), Vector2<int>(600, 200), Color::red);
		DrawLine(Vector2<int>(-100, 100), Vector2<int>(100, 100), Color::green);
	}

	void DrawLine(Vector2<int> start, Vector2<int> end, const Color& color) {

		bool clipResult = cohenSutherlandLineClip(start, end, Vector2<int>(w - 1, h - 1));
		if (clipResult) {
			drawLineWithoutClip(start, end, color);
		}
	}


private:

	//设置像素颜色
	void setColor(int x, int y, const Color& color) const;

	//bresenman算法画线
	void drawLineWithoutClip(const Vector2<int>& start, const Vector2<int>& end, const Color& color) const;

	//cohenSutherland线裁剪
	bool cohenSutherlandLineClip(Vector2<int>& v0, Vector2<int>& v1, const Vector2<int>& max)
	{
		int outcode0 = ComputeOutCode(v0, max);
		int outcode1 = ComputeOutCode(v1, max);
		bool accept = false;

		while (true) {
			if (!(outcode0 | outcode1)) {
				//都符合
				accept = true;
				break;
			}
			else if (outcode0 & outcode1) {
				//都在外边
				break;
			}
			else {
				int x, y;

				//code 有1的就是在外面，比0大,
				int outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

				if (outcodeOut & TOP) {           // point is above the clip window
					x = v0.x + (v1.x - v0.x) * (max.y - v0.y) / (v1.y - v0.y);
					y = max.y;
				}
				else if (outcodeOut & BOTTOM) { // point is below the clip window
					x = v0.x + (v1.x - v0.x) * (0 - v0.y) / (v1.y - v0.y);
					y = 0;
				}
				else if (outcodeOut & RIGHT) {  // point is to the right of clip window
					y = v0.y + (v1.y - v0.y) * (max.x - v0.x) / (v1.x - v0.x);
					x = max.x;
				}
				else if (outcodeOut & LEFT) {   // point is to the left of clip window
					y = v0.y + (v1.y - v0.y) * (0 - v0.x) / (v1.x - v0.x);
					x = 0;
				}

				if (outcodeOut == outcode0) {
					v0.x = x;
					v0.y = y;
					outcode0 = ComputeOutCode(v0, max);
				}
				else {
					v1.x = x;
					v1.y = y;
					outcode1 = ComputeOutCode(v1, max);
				}
			}
		}
		return accept;
	}

	int INSIDE = 0; // 0000
	int LEFT = 1;   // 0001
	int RIGHT = 2;  // 0010
	int BOTTOM = 4; // 0100
	int TOP = 8;    // 1000

	inline int ComputeOutCode(Vector2<int> v, const Vector2<int>& max) const
	{
		int code = INSIDE;  // initialised as being inside of clip window

		if (v.x < 0)           // to the left of clip window
			code |= LEFT;
		else if (v.x > max.x)      // to the right of clip window
			code |= RIGHT;
		if (v.y < 0)           // below the clip window
			code |= BOTTOM;
		else if (v.y > max.y)      // above the clip window
			code |= TOP;

		return code;
	}


};

#endif 
