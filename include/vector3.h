#ifndef _VECTOR_3_H
#define _VECTOR_3_H

template<typename T>
struct Vector3 {

public:
	T x;
	T y;
	T z;

	Vector3(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

#endif
