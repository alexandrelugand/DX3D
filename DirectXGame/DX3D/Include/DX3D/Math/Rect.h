#pragma once
#include <Windows.h>

class Rect
{
public:
	Rect()
		: left(0), right(0), top(0), bottom(0) { }

	Rect(int width, int height)
		: left(0), right(width), top(0), bottom(height) { }

	Rect(int left, int right, int top, int bottom)
		: left(left), right(right), top(top), bottom(bottom) { }

	Rect(const RECT& r)
		: left(r.left), right(r.right), top(r.top), bottom(r.bottom) { }

	int Width() const
	{
		return right - left;
	}

	int Height() const
	{
		return bottom - top;
	}

	int left;
	int right;
	int top;
	int bottom;
};
