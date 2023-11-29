#include <cmath>
#include <algorithm>

#include "cpu_renderer.h"

void CPURenderer::setColor(int x, int y, const Color& color) const
{
	int index = (x + y * w) * channels;
	for (int i = 0; i < channels; i++)
	{
		colorAttachment[index + 0] = color.r;
		colorAttachment[index + 1] = color.g;
		colorAttachment[index + 2] = color.b;
	}
}

void CPURenderer::bresenmanDrawLine(const Vector2<int>& start, const Vector2<int>& end, const Color& color) const
{
	int x1 = start.x, y1 = start.y;
	int x2 = end.x, y2 = end.y;

	int dx = std::abs(x2 - x1);
	int dy = std::abs(y2 - y1);

	int sx = (x1 < x2) ? 1 : -1;
	int sy = (y1 < y2) ? 1 : -1;

	int err = dx - dy;

	while (x1 != x2 || y1 != y2) {
		setColor(x1, y1, color);

		int err2 = 2 * err;

		if (err2 > -dy) {
			err -= dy;
			x1 += sx;
		}

		if (err2 < dx) {
			err += dx;
			y1 += sy;
		}
	}

	setColor(x2, y2, color);
}

int INSIDE = 0; // 0000
int LEFT = 1;   // 0001
int RIGHT = 2;  // 0010
int BOTTOM = 4; // 0100
int TOP = 8;    // 1000

bool CPURenderer::cohenSutherlandLineClip(Vector2<int>& v0, Vector2<int>& v1, const Vector2<int>& max)
{
	int outcode0 = cohenSutherlandLineComputeOutCode(v0, max);
	int outcode1 = cohenSutherlandLineComputeOutCode(v1, max);
	bool accept = false;

	while (true) {
		if (!(outcode0 | outcode1)) {
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) {
			break;
		}
		else {
			int x, y;

			int outcodeOut = outcode1 > outcode0 ? outcode1 : outcode0;

			if (outcodeOut & TOP) {
				x = v0.x + (v1.x - v0.x) * (max.y - v0.y) / (v1.y - v0.y);
				y = max.y;
			}
			else if (outcodeOut & BOTTOM) {
				x = v0.x + (v1.x - v0.x) * (0 - v0.y) / (v1.y - v0.y);
				y = 0;
			}
			else if (outcodeOut & RIGHT) {
				y = v0.y + (v1.y - v0.y) * (max.x - v0.x) / (v1.x - v0.x);
				x = max.x;
			}
			else if (outcodeOut & LEFT) {
				y = v0.y + (v1.y - v0.y) * (0 - v0.x) / (v1.x - v0.x);
				x = 0;
			}

			if (outcodeOut == outcode0) {
				v0.x = x;
				v0.y = y;
				outcode0 = cohenSutherlandLineComputeOutCode(v0, max);
			}
			else {
				v1.x = x;
				v1.y = y;
				outcode1 = cohenSutherlandLineComputeOutCode(v1, max);
			}
		}
	}
	return accept;
}

int CPURenderer::cohenSutherlandLineComputeOutCode(const Vector2<int>& v, const Vector2<int>& max) const
{
	int code = INSIDE;

	if (v.x < 0)
		code |= LEFT;
	else if (v.x > max.x)
		code |= RIGHT;
	if (v.y < 0)
		code |= BOTTOM;
	else if (v.y > max.y)
		code |= TOP;

	return code;
}