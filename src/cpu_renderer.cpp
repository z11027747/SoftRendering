#include <cmath>
#include <algorithm>

#include "cpu_renderer.h"

void CPURenderer::setColor(int x, int y, const Color& color) const {

	//std::cout << " setColor  =======================> ";
	//std::cout << "x: " << x << ", y: " << y;
	//std::cout << "\n";

	//TODO
	if (x < 0 || x >= viewport.w || y < 0 || y >= viewport.h)
		return;

	int index = (x + y * viewport.w) * 3;
	colorAttachment[index + 0] = color.r;
	colorAttachment[index + 1] = color.g;
	colorAttachment[index + 2] = color.b;
}

void CPURenderer::clearColor() {
	for (int i = 0; i < colorLen; i++)
	{
		colorAttachment[i] = 0;
	}
}

void CPURenderer::bresenmanDrawLine(const Vector2<int>& start, const Vector2<int>& end,
	const Color& color) const
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

bool CPURenderer::cohenSutherlandLineClip(Vector2<int>& v0, Vector2<int>& v1, const Vector2<int>& max) const
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

	if (v.x < 0) code |= LEFT;
	else if (v.x > max.x) code |= RIGHT;
	if (v.y < 0) code |= BOTTOM;
	else if (v.y > max.y) code |= TOP;

	return code;
}

int CPURenderer::splitTrapezoids(const Vector2<int>& v1, const Vector2<int>& v2, const Vector2<int>& v3,
	Trapezoid& trapezoid1, Trapezoid& trapezoid2)  const {

	//y排序
	std::vector<Vector2<int>> vertices{ v1, v2, v3 };
	std::sort(vertices.begin(), vertices.end(),
		[&](Vector2<int> lft, Vector2<int> rhd) -> bool { return lft.y > rhd.y; });

	////共线
	//if ((vertices[0].x == vertices[1].x && vertices[0].x == vertices[2].x)
	//	|| (vertices[0].y == vertices[1].y && vertices[0].y == vertices[2].y))
	//{
	//	return 0;
	//}

	//std::cout << "v[0]y: " << vertices[0].y << ", v[1]y: " << vertices[1].y << ", v[2]y: " << vertices[2].y;
	//std::cout << "\n";

	//上平底
	// 0 1
	//  2
	if (vertices[0].y == vertices[1].y) {
		trapezoid1.top = vertices[0].y;
		trapezoid1.bottom = vertices[2].y;
		trapezoid1.ld = trapezoid1.rd = vertices[2];
		trapezoid1.lt = vertices[0];
		trapezoid1.rt = vertices[1];

		return 1;
	}

	//下平底
	//  0
	// 1 2
	if (vertices[1].y == vertices[2].y) {
		trapezoid1.top = vertices[0].y;
		trapezoid1.bottom = vertices[2].y;
		trapezoid1.lt = trapezoid1.rt = vertices[0];
		trapezoid1.ld = vertices[1];
		trapezoid1.rd = vertices[2];

		return 1;
	}

	//直线方程，求中点平底x
	float radio = (float)(vertices[0].y - vertices[1].y) / (float)(vertices[0].y - vertices[2].y);
	int xmid = vertices[0].x + (int)std::floor(radio * (vertices[2].x - vertices[0].x));

	//std::cout << "v[0]x: " << vertices[0].x << ", v[1]x: " << vertices[1].x << ", v[2]x: " << vertices[2].x;
	//std::cout << "\n";
	//std::cout << "xmid: " << xmid;
	//std::cout << "\n";

	//判断左右
	if (xmid < vertices[1].x) {
		trapezoid1.top = vertices[0].y;
		trapezoid1.bottom = vertices[1].y;
		trapezoid1.lt = trapezoid1.rt = vertices[0];
		trapezoid1.ld = vertices[1];
		trapezoid1.rd = Vector2<int>(xmid, vertices[1].y);

		trapezoid2.top = vertices[1].y;
		trapezoid2.bottom = vertices[2].y;
		trapezoid2.ld = trapezoid2.rd = vertices[2];
		trapezoid2.lt = vertices[1];
		trapezoid2.rt = Vector2<int>(xmid, vertices[1].y);

		return 2;
	}
	else {
		trapezoid1.top = vertices[0].y;
		trapezoid1.bottom = vertices[1].y;
		trapezoid1.lt = trapezoid1.rt = vertices[0];
		trapezoid1.ld = Vector2<int>(xmid, vertices[1].y);
		trapezoid1.rd = vertices[1];

		trapezoid2.top = vertices[1].y;
		trapezoid2.bottom = vertices[2].y;
		trapezoid2.ld = trapezoid2.rd = vertices[2];
		trapezoid2.lt = Vector2<int>(xmid, vertices[1].y);
		trapezoid2.rt = vertices[1];

		return 2;
	}
}
void CPURenderer::drawTrapezoid(const Trapezoid& trapezoid,
	const Color& color) const
{
	int y = trapezoid.top;
	while (y >= trapezoid.bottom) {
		Scanline scanline = genScanline(trapezoid, y);

		int start = scanline.start;
		int width = scanline.width;
		int step = width > 0 ? 1 : -1;

		while (width != 0) {
			setColor(start, y, color);

			width -= step;
			start += step;
		}

		y -= 1;
	}
}

Scanline CPURenderer::genScanline(const Trapezoid& trapezoid, int beginY) const {

	//左交点
	float lt = (float)(beginY - trapezoid.ld.y) / (float)(trapezoid.lt.y - trapezoid.ld.y);
	int lx = (int)std::floor(trapezoid.ld.x + (trapezoid.lt.x - trapezoid.ld.x) * lt);

	//右交点
	float rt = (beginY - trapezoid.rd.y) * 1.0f / (trapezoid.rt.y - trapezoid.rd.y);
	int rx = (int)std::floor(trapezoid.rd.x + (trapezoid.rt.x - trapezoid.rd.x) * rt);

	Scanline scanline;
	scanline.start = lx;
	scanline.width = (rx - lx);

	//std::cout << " genScanline  =======================> ";
	//std::cout << "start: " << scanline.start << ", width: " << scanline.width;
	//std::cout << "\n";

	return scanline;
}