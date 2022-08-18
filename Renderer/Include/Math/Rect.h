#pragma once
#include "Vector2.h"

template<typename T>
class Rect
{
public:
	T X, Y, Width, Height;

public:
	Rect(T x, T y, T width, T height) : X(x), Y(y), Width(width), Height(height)
	{}

public:
	T Left()const { return X; }
	T Right()const { return X + Width; }
	T Top()const { return Y + Height; }
	T Bottom()const { return Y; }
	bool Contains(const Vector2& point)const
	{
		return (point.x > X && point.x < X + Width) && (point.y < Y + Height && point.y > Y);
	}
};