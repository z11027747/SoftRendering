#ifndef _STATIC_HELPER_H
#define _STATIC_HELPER_H

#include "vector.h"
#include "all_define.h"
#include "image/stb_image.h"

#define PI 3.1415926

class Helper {
public:

	static float Rad2Deg(float rad) {
		return (rad * PI) / 180.0f;
	}

	template <typename U>
	static U Lerp(const U& l, const U& r, const float t) {

		return l + (r - l) * t;
	}

	template <typename U>
	static Vector2<U> Lerp2(const Vector2<U>& l, const Vector2<U>& r, float t) {

		Vector2<U> result;
		result.x = Lerp(l.x, r.x, t);
		result.y = Lerp(l.y, r.y, t);
		return result;
	}

	template <typename U>
	static Vector3<U> Lerp3(const Vector3<U>& l, const Vector3<U>& r, float t) {

		Vector3<U> result;
		result.x = Lerp(l.x, r.x, t);
		result.y = Lerp(l.y, r.y, t);
		result.z = Lerp(l.z, r.z, t);
		return result;
	}

	template <typename U>
	static Vector4<U> Lerp4(const Vector4<U>& l, const Vector4<U>& r, float t) {

		Vector4<U> result;
		result.x = Lerp(l.x, r.x, t);
		result.y = Lerp(l.y, r.y, t);
		result.z = Lerp(l.z, r.z, t);
		result.w = Lerp(l.w, r.w, t);
		return result;
	}

	static Color LerpColor(const Color& l, const Color& r, float t) {

		Color result;
		result.r = Lerp(l.r, r.r, t);
		result.g = Lerp(l.g, r.g, t);
		result.b = Lerp(l.b, r.b, t);
		return result;
	}

	static Vertex LerpVertex(const Vertex& l, const Vertex& r, float t) {

		Vertex result;
		result.position = Lerp4(l.position, r.position, t);
		result.color = LerpColor(l.color, r.color, t);
		result.uv = Lerp2(l.uv, r.uv, t);
		return result;
	}

	static Texture LoadImage(const char* name) {

		stbi_set_flip_vertically_on_load(true);

		int w, h, c;
		unsigned char* data = stbi_load(name, &w, &h, &c, 0);

		//std::cout << " LoadImage  =======================> ";
		//std::cout << "w: " << w << ", h: " << h << ", c: " << c << "\n";

		Texture result;
		result.w = w;
		result.h = h;
		result.c = c;
		result.data = data;

		return result;
	}
};


#endif // !_STATIC_HELPER_H
