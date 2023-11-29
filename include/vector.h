#ifndef _VECTOR_3_H
#define _VECTOR_3_H

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
};

#endif
