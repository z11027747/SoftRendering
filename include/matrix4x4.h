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
	void SetR_X(float theta);
	void SetR_Y(float theta);
	void SetS(float x, float y, float z);

	friend Vector4<float> operator* (const Matrix4x4& m, const Vector4<float>& p);
	friend Matrix4x4 operator* (const Matrix4x4& a, const Matrix4x4& b);
};


#endif // !_MATRIX_H
