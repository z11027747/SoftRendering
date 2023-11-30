#ifndef _VECTOR_3_H
#define _VECTOR_3_H

#include<iostream>

struct Vector2 {

public:
	int x;
	int y;

	Vector2() {
		x = y = 0;
	}

	Vector2(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void Print(const char* name) const {
		std::cout << name << ": " << "x: " << x << ", y: " << y << "\n";
	}
};

struct Vector3 {

public:
	float x;
	float y;
	float z;

	Vector3() {
		x = y = z = 0;
	}

	Vector3(float x, float y, float z)
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

struct Vector4 {

public:
	float x;
	float y;
	float z;
	float w;

	Vector4() {
		x = y = z = w = 0;
	}

	Vector4(const Vector3& v3, float w)
	{
		this->x = v3.x;
		this->y = v3.y;
		this->z = v3.z;
		this->w = w;
	}

	Vector4(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	friend Vector4 operator/ (const Vector4& m, float v) {
		Vector4 result;
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
