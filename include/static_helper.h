#ifndef _STATIC_HELPER_H
#define _STATIC_HELPER_H

#include "vector.h"
#include "all_define.h"

#define PI 3.1415926

class Helper {
public:

	static float Rad2Deg(float rad) {
		return (rad * PI) / 180.0f;
	}

	static float Lerp(const float l, const float r, const float t) {

		float result;
		result = l + (r - l) * t;
		return result;
	}

	template <typename U>
	static Vector3<U> Lerp3(const Vector3<U>& l, const Vector3<U>& r, float t) {

		Vector3<U> result;
		result.x = l.x + (r.x - l.x) * t;
		result.y = l.y + (r.y - l.y) * t;
		result.z = l.z + (r.z - l.z) * t;
		return result;
	}

	template <typename U>
	static Vector4<U> Lerp4_XY(const Vector4<U>& l, const Vector4<U>& r, float t) {

		Vector4<U> result;
		result.x = l.x + (r.x - l.x) * t;
		result.y = l.y + (r.y - l.y) * t;
		return result;
	}

	static Color LerpColor(const Color& l, const Color& r, float t) {

		Color result;
		result.r = l.r + (unsigned char)((float)(r.r - l.r) * t);
		result.g = l.g + (unsigned char)((float)(r.g - l.g) * t);
		result.b = l.b + (unsigned char)((float)(r.b - l.b) * t);
		return result;
	}

	static Vertex LerpVertex(const Vertex& l, const Vertex& r, float t) {

		Vertex result;
		result.position = Lerp4_XY(l.position, r.position, t);
		result.color = LerpColor(l.color, r.color, t);
		return result;
	}
};


#endif // !_STATIC_HELPER_H
