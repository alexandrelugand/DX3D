#pragma once

class Point
{
public:
	Point()
		: x(0), y(0) { }

	Point(int x, int y)
		: x(x), y(y) { }

	Point(const Point& p)
		: x(p.x), y(p.y) { }

	int x = 0;
	int y = 0;
};
