#ifndef _VECTOR_2_H
#define _VECTOR_2_H

template<typename T>
struct Vector2 {

public:
	T x;
	T y;

	Vector2(T x, T y)
	{
		this->x = x;
		this->y = y;
	}
};

#endif
