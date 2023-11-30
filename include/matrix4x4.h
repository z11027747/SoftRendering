#ifndef _MATRIX_H
#define _MATRIX_H

#include<iostream>

#include "vector.h"

struct Matrix4x4 {

	float m11, m12, m13, m14;
	float m21, m22, m23, m24;
	float m31, m32, m33, m34;
	float m41, m42, m43, m44;

	Matrix4x4() {
		Zero();
	}

	void Zero();
	void Identity();

	void SetT(float x, float y, float z);
	void SetR_Y(float theta);
	void SetS(float x, float y, float z);

	friend Vector4 operator* (const Matrix4x4& m, const Vector4& p);
	friend Matrix4x4 operator* (const Matrix4x4& a, const Matrix4x4& b);

	void Print(const char* name) const {
		std::cout << "===============>" << name << ": " << "\n";
		std::cout << "m11: " << m11 << ", m12: " << m12 << ", m13: " << m13 << ", m14: " << m14 << "\n";
		std::cout << "m21: " << m21 << ", m22: " << m22 << ", m23: " << m23 << ", m24: " << m24 << "\n";
		std::cout << "m31: " << m31 << ", m32: " << m32 << ", m33: " << m33 << ", m34: " << m34 << "\n";
		std::cout << "m41: " << m41 << ", m42: " << m42 << ", m43: " << m43 << ", m44: " << m44 << "\n";
	}
};


#endif // !_MATRIX_H
