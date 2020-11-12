#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

#define X_MAX 1200
#define Y_MAX 650

#define X_STEP 50
#define Y_STEP 50

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
	Path
};
