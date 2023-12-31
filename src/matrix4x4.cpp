#include "matrix4x4.h"

#include <cmath>

void Matrix4x4::Zero() {
	m11 = 0.0f; m12 = 0.0f;  m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f; m22 = 0.0f;  m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f;  m33 = 0.0f; m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f;  m43 = 0.0f; m44 = 0.0f;
}

void Matrix4x4::Identity() {
	m11 = 1.0f; m12 = 0.0f;  m13 = 0.0f; m14 = 0.0f;
	m21 = 0.0f; m22 = 1.0f;  m23 = 0.0f; m24 = 0.0f;
	m31 = 0.0f; m32 = 0.0f;  m33 = 1.0f; m34 = 0.0f;
	m41 = 0.0f; m42 = 0.0f;  m43 = 0.0f; m44 = 1.0f;
}

void Matrix4x4::SetT(float x, float y, float z) {
	m14 = x;
	m24 = y;
	m34 = z;
}

void Matrix4x4::SetR_X(float radians) {

	float sin = std::sin(radians);
	float cos = std::cos(radians);

	m22 = cos; m23 = -sin;
	m32 = sin; m33 = cos;
}

void Matrix4x4::SetR_Y(float radians) {

	float sin = std::sin(radians);
	float cos = std::cos(radians);

	m11 = cos; m13 = sin;
	m31 = -sin; m33 = cos;
}

void Matrix4x4::SetS(float x, float y, float z) {
	m11 *= x;
	m22 *= y;
	m33 *= z;
}

Vector4<float> operator*(const Matrix4x4& m, const Vector4<float>& p)
{
	Vector4<float> result;
	result.x = p.x * m.m11 + p.y * m.m12 + p.z * m.m13 + p.w * m.m14;
	result.y = p.x * m.m21 + p.y * m.m22 + p.z * m.m23 + p.w * m.m24;
	result.z = p.x * m.m31 + p.y * m.m32 + p.z * m.m33 + p.w * m.m34;
	result.w = p.x * m.m41 + p.y * m.m42 + p.z * m.m43 + p.w * m.m44;

	return result;
}

Matrix4x4 operator*(const Matrix4x4& a, const Matrix4x4& b)
{
	Matrix4x4 result;
	result.m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
	result.m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
	result.m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
	result.m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;

	result.m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
	result.m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
	result.m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
	result.m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;

	result.m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
	result.m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
	result.m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
	result.m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;

	result.m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;
	result.m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;
	result.m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;
	result.m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;

	return result;
}