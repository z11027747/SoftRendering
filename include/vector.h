﻿#ifndef _VECTOR_3_H
#define _VECTOR_3_H

#include <iostream>
#include <math.h>

template <typename T>
struct Vector2 {

	T x;
	T y;

	Vector2() {
		x = y = T();
	}

	Vector2(T x, T y) {
		this->x = x;
		this->y = y;
	}

	friend Vector2<T> operator/ (const Vector2<T>& m, float v) {
		Vector2<T> result;
		result.x = m.x / v;
		result.y = m.y / v;

		return result;
	}
};

template <typename T>
struct Vector3 {

	T x;
	T y;
	T z;

	Vector3() {
		x = y = z = T();
	}

	Vector3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3<T>& Normalize() {

		float len = (x * x) + (y * y) + (z * z);
		float ooLen = 1.0f / std::sqrt(len);

		x *= ooLen;
		y *= ooLen;
		z *= ooLen;

		return *this;
	}

	Vector3<T> Cross(const Vector3<T>& m) const {
		Vector3<float> crossValue;
		crossValue.x = (y * m.z) - (z * m.y);
		crossValue.y = (z * m.x) - (x * m.z);
		crossValue.z = (x * m.y) - (y * m.x);

		return crossValue;
	}

	float Dot(const Vector3<T>& m) const {
		float dotValue = (x * m.x) + (y * m.y) + (z * m.z);

		return dotValue;
	}
};

template <typename T>
struct Vector4 {

	T x;
	T y;
	T z;
	T w;

	Vector4() {
		x = y = z = w = T();
	}

	Vector4(const Vector3<T>& v3, T w)
	{
		this->x = v3.x;
		this->y = v3.y;
		this->z = v3.z;
		this->w = w;
	}

	Vector4(T x, T y, T z, T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	friend Vector4<T> operator/ (const Vector4<T>& m, float v) {
		Vector4<T> result;
		result.x = m.x / v;
		result.y = m.y / v;
		result.z = m.z / v;
		result.w = m.w / v;

		return result;
	}
};

#endif
