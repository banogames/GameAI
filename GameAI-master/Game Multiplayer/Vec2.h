
#include <stdlib.h>    

class Vec2
{
public:

	int x;
	int y;

	Vec2() : x(0), y(0)
	{
	}

	Vec2(int x1, int y1) : x(x1), y(y1)
	{
	}

	void reset(int x1, int y1)
	{
		x = x1;
		y = y1;
	}

	int distance(const Vec2 &other) const
	{
		return abs(other.x - x) + abs(other.y - y);
	}

	bool operator== (const Vec2 &other) const
	{
		return x == other.x && y == other.y;
	}
};

