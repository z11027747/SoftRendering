#include <cmath>
#include <algorithm>

#include "cpu_renderer.h"

void CPURenderer::drawLineWithoutClip(const Vector2<int>& start, const Vector2<int>& end, const Color& color) const
{
	int dx = std::abs(end.x - start.x);
	int dy = std::abs(end.y - start.y);

	int sx = end.x >= start.x ? 1 : -1;
	int sy = end.y >= start.y ? 1 : -1;

	int x = start.x;
	int y = start.y;

	int dir = dx < dy ? 1 : -1;
	int result = dx < dy ? end.y : end.x;

	if (dx < dy)
	{
		std::swap(dx, dy);
		std::swap(x, y);
		std::swap(sx, sy);
	}

	int e = -dx;
	int step = 2 * dy;
	int desc = -2 * dx;

	while (x != result)
	{
		if (dir > 0) {
			setColor(y, x, color);
		}
		else {
			setColor(x, y, color);
		}

		e += step;
		if (e >= 0) {
			y += sy;
			e += desc;
		}

		x += sx;
	}
}

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