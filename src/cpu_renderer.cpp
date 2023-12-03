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

int CPURenderer::cohenSutherlandLineComputeOutCode(const Vector2<int>& v, const Vector2<int>& max) const {

	int code = INSIDE;

	if (v.x < 0) code |= LEFT;
	else if (v.x > max.x) code |= RIGHT;
	if (v.y < 0) code |= BOTTOM;
	else if (v.y > max.y) code |= TOP;

	return code;
}

int CPURenderer::splitTrapezoids(
	const Vertex& v1, const Vertex& v2, const Vertex& v3,
	Trapezoid* trapezoids)  const
{
	//y排序
	std::vector<Vertex> vertices = { v1, v2, v3 };
	std::sort(vertices.begin(), vertices.end(),
		[&](Vertex lft, Vertex rhd) -> bool { return lft.position.y > rhd.position.y; });

	//std::cout << "v[0]y: " << vertices[0].y << ", v[1]y: " << vertices[1].y << ", v[2]y: " << vertices[2].y;
	//std::cout << "\n";

	//上平底
	// 0 1
	//  2
	if (vertices[0].position.y == vertices[1].position.y) {
		trapezoids[0].ld = trapezoids[0].rd = vertices[2];
		trapezoids[0].lt = vertices[0];
		trapezoids[0].rt = vertices[1];

		return 1;
	}

	//下平底
	//  0
	// 1 2
	if (vertices[1].position.y == vertices[2].position.y) {
		trapezoids[0].lt = trapezoids[0].rt = vertices[0];
		trapezoids[0].ld = vertices[1];
		trapezoids[0].rd = vertices[2];

		return 1;
	}

	//直线方程，求中点平底x
	float t = (vertices[0].position.y - vertices[1].position.y) / (vertices[0].position.y - vertices[2].position.y);
	Vertex mid = Helper::LerpVertex(vertices[0], vertices[2], t);

	//std::cout << " splitTrapezoids  =======================> ";
	//std::cout << "\n";
	//mid.position.Print("mid.position");

	//判断左右
	if (mid.position.x < vertices[1].position.x) {
		trapezoids[0].lt = trapezoids[0].rt = vertices[0];
		trapezoids[0].ld = mid;
		trapezoids[0].rd = vertices[1];

		trapezoids[1].ld = trapezoids[1].rd = vertices[2];
		trapezoids[1].lt = mid;
		trapezoids[1].rt = vertices[1];

		//std::cout << " splitTrapezoids  =======================> ";
		//std::cout << "\n";
		//mid.position.Print("mid.position");
		//trapezoids[0].ld.position.Print("trapezoids[0].ld.position");
		//trapezoids[0].rd.position.Print("trapezoids[0].rd.position");
		//std::cout << "\n";
		//trapezoids[1].lt.position.Print("trapezoids[1].lt.position");
		//trapezoids[1].rt.position.Print("trapezoids[1].rt.position");
		//std::cout << "\n";

		return 2;
	}
	else {
		trapezoids[0].lt = trapezoids[0].rt = vertices[0];
		trapezoids[0].ld = vertices[1];
		trapezoids[0].rd = mid;

		trapezoids[1].ld = trapezoids[1].rd = vertices[2];
		trapezoids[1].lt = vertices[1];
		trapezoids[1].rt = mid;

		return 2;
	}
}

void CPURenderer::drawTrapezoid(const Trapezoid& trapezoid) const
{
	//std::cout << " drawTrapezoid  =======================> ";
	//std::cout << "\n";
	//trapezoid.ld.position.Print("trapezoid.ld.position");
	//trapezoid.rd.position.Print("trapezoid.rd.position");
	//std::cout << "\n";

	int yt = (int)trapezoid.lt.position.y;
	int yd = (int)trapezoid.ld.position.y;

	while (yt >= yd) {
		Scanline scanline = genScanline(trapezoid, yt);

		Vertex& left = scanline.left;
		Vertex& right = scanline.right;

		float t = 0.0f;
		float step = scanline.step;

		while (t <= 1.0f) {

			Vertex curr = Helper::LerpVertex(left, right, t);
			t += step;

			//std::cout << "		drawScanline  =======================> ";
			//std::cout << "\n";
			//std::cout << "t: " << t;
			//std::cout << "\n";
			//curr.position.Print("curr.position");
			//std::cout << "\n";

			//采样贴图
			Color texCol = texture.Tex(curr.uv);

			setColor((int)curr.position.x, yt, texCol);
		}

		yt -= 1;
	}
}

Scanline CPURenderer::genScanline(const Trapezoid& trapezoid, int y) const {

	//相似三角形比例
	float lt = (y - (int)trapezoid.ld.position.y) * 1.0f
		/ ((int)trapezoid.lt.position.y - (int)trapezoid.ld.position.y);
	float rt = (y - (int)trapezoid.rd.position.y) * 1.0f
		/ ((int)trapezoid.rt.position.y - (int)trapezoid.rd.position.y);

	//左右交点
	Vertex left = Helper::LerpVertex(trapezoid.ld, trapezoid.lt, lt);
	Vertex right = Helper::LerpVertex(trapezoid.rd, trapezoid.rt, rt);

	Scanline scanline;
	scanline.left = left;
	scanline.right = right;
	scanline.step = 1.0f / (right.position.x - left.position.x);

	if (scanline.step < 0.0f) {
		//std::cout << " genScanline  =======================> ";
		//std::cout << "\n";
		//std::cout << "y: " << y << ", step: " << scanline.step;
		//std::cout << "\n";
		//std::cout << "lt: " << lt << ", rt: " << rt;
		//std::cout << "\n";
		//trapezoid.ld.position.Print("trapezoid.ld.position");
		//trapezoid.lt.position.Print("trapezoid.lt.position");
		//trapezoid.rt.position.Print("trapezoid.rt.position");
		//scanline.left.position.Print("scanline.left.position");
		//scanline.right.position.Print("scanline.right.position");
		//std::cout << "\n";
	}

	//std::cout << " genScanline  =======================> ";
	//std::cout << "\n";
	//Vertex mid = Helper::LerpVertex(left, right, 0.5f);
	//mid.position.Print("mid.position");
	//mid.color.Print("mid.color");
	//std::cout << "\n";
	//std::cout << "\n";

	return scanline;
}