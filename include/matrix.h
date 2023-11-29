#ifndef _MATRIX_H
#define _MATRIX_H

struct Matrix4x4 {

	float(*value)[4];

	Matrix4x4() {
		value = new float[4][4]{
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f, 0.0f },
		};
	}

	~Matrix4x4() {
		delete[] value;
	}

	static Matrix4x4 identity() {
		Matrix4x4 result = Matrix4x4();
		result[0][0] = 1;
		result[1][1] = 1;
		result[2][2] = 1;
		result[3][3] = 1;

		return result;
	}

	//{ 0, 0, 0, 0 }  { 0, 0, 0, 0 }
	//{ 0, 0, 0, 0 }  { 0, 0, 0, 0 }
	//{ 0, 0, 0, 0 }  { 0, 0, 0, 0 }
	//{ 0, 0, 0, 0 }  { 0, 0, 0, 0 }
	void Mul(const Matrix4x4& other)
	{
		Matrix4x4 result = Matrix4x4::identity();

	}


};


#endif // !_MATRIX_H
