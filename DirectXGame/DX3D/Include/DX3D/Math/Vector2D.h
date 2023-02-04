#pragma once

class Vector2D
{
public:
	Vector2D()
		: x(0), y(0) { }

	Vector2D(float x, float y)
		: x(x), y(y) { }

	Vector2D(const Vector2D& v)
		: x(v.x), y(v.y) { }

	Vector2D operator *(float num) const
	{
		return Vector2D(x * num, y * num);
	}

	Vector2D operator +(const Vector2D& vec) const
	{
		return Vector2D(x + vec.x, y + vec.y);
	}

	Vector2D operator -(const Vector2D& vec) const
	{
		return Vector2D(x - vec.x, y - vec.y);
	}

	float x, y;
};
