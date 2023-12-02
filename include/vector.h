#ifndef _VECTOR_3_H
#define _VECTOR_3_H

#include<iostream>

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

	void Print(const char* name) const {
		std::cout << name << ": " << "x: " << x << ", y: " << y << "\n";
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

	void Print(const char* name) const {
		std::cout << name << ": " << "\n";
		std::cout << "x: " << x << ", y: " << y << ", z: " << z << "\n";
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

	void Print(const char* name) const {
		std::cout << name << ": " << "\n";
		std::cout << "x: " << x << ", y: " << y << ", z: " << z << ", w: " << w << "\n";
	}
};

#endif
