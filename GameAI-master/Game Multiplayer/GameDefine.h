#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#define X_MAX 1280
#define Y_MAX 800

#define X_STEP 32
#define Y_STEP 32

//count npc, player
const int COUNT_PLAYER_SECURITY = 5;
const int COUNT_PLAYER_OTHER = 5;

const int COUNT_NPC_FAST = 12;
const int COUNT_NPC_TANK = 6;
const int COUNT_NPC_SECURITY = 5;

const int COUNT_PROTECT_ITEM = 20;
const int COUNT_HEAL_ITEM = 20;
const float TIME_SPAWN_ITEM = 1.f;
const float TIME_SHIELD = 10.f; //10 s được bảo vệ

const int COUNT_X = 120; //X_MAX / X_STEP;
const int COUNT_Y = 120; //Y_MAX / Y_STEP;

const int SIZE_MAP_X = COUNT_X * X_STEP;
const int SIZE_MAP_Y = COUNT_Y * Y_STEP;

const int SIZE_PART = 12; //10 tọa độ 1 ô (chia nhỏ map)

const int NUM_PART_X = COUNT_X / SIZE_PART;
const int NUM_PART_Y = COUNT_X / SIZE_PART;

const bool IS_DRAW_PATH_ASTAR = true; //test

const int VALUE_ASTAR_PLAYER = 1;
const int VALUE_ASTAR_NPC = 1;
const int VALUE_ASTAR_BRICK = 1;
const int VALUE_ASTAR_EAGLE = 1;
const int VALUE_ASTAR_ITEM = 0;

// [0, 10] => 4 bit
enum EntityType
{
	ET_Unknown,
	ET_Player, 
	ET_NPC,
	ET_Bullet,
	ET_Boundary, 
	ET_NormalBrick, 
	ET_MetalBrick, 
	ET_Water,
	ET_Grass,
	ET_ProtectItem, 
	ET_UpgradeItem,
	ET_HealItem,
	ET_EaglePlayer,
	ET_EagleNPC
};

enum NPCType 
{
	NPC_FAST,
	NPC_TANK,
	NPC_SECURITY
};

enum BulletType 
{
	BL_Player,
	BL_NPC
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
	Path_Dodging,
	Path_Attacking
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

