#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#define X_MAX 1280
#define Y_MAX 800

#define X_STEP 32
#define Y_STEP 32

const int COUNT_X = X_MAX / X_STEP;
const int COUNT_Y = Y_MAX / Y_STEP;


// [0, 10] => 4 bit
enum EntityType
{
	ET_Unknown,
	ET_Player, 
	ET_Bullet,
	ET_Boundary, 
	ET_NormalBrick, 
	ET_MetalBrick, 
	ET_Water,
	ET_ProtectItem, 
	ET_UpgradeItem,
	ET_NPC,
	ET_Eagle
};

// [0, 4] => 3 bit
enum Direction
{
	D_Stand,
	D_Left,
	D_Right,
	D_Up,
	D_Down
};

enum TileType
{
	Empty,
	Opened,
	Closed,
	Obstacle,
	Moving,
	Begin,
	Destination,
	Path,
	Path_Dodging
};

struct  Vec2
{
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

