#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include <array>

#include "Sprite.h"	
#include "MapReader\Tmx.h.in"
#include "GameGlobal.h"
#include "Brick.h"
#include "BrickNormal.h"
#include "MetalBrick.h"
#include "Water.h"
#include "Boundary.h"
#include "Eagle.h"
#include "Grass.h"
#include "GameCollision.h"
#include "GameDebugDraw.h"
#include "QuadTree.h"

class GameMap
{
	//mảng map bricks
	std::array < std::array<Brick*, COUNT_Y>, COUNT_X> _bricks;
	std::array < std::array<Grass*, COUNT_Y>, COUNT_X> _grassies;

	std::vector<Brick*> _brickList;	
	std::vector<BrickNormal*> _brickNorList;
	std::vector<Eagle*> _eagleList;
	std::vector<Grass*> _grassList;
	Tmx::Map *_map;
	std::map<int, Sprite*>  _tilesetList;
	GameDebugDraw* _debugDraw;

	void LoadMap(char* filePath);
	int getWidth() { return _map->GetWidth() * _map->GetTileWidth(); }
	int getHeight() { return _map->GetHeight() * _map->GetTileHeight(); }
	int getTileWidth() { return _map->GetTileWidth(); }
	int getTileHeight() { return _map->GetTileHeight(); }

public:
	GameMap(char* filePath);
	~GameMap() {}
	void Draw();
	void DrawInCamera(int posXMin, int posXMax, int posYMin, int posYMax);
	void DrawGrassInCamera(int posXMin, int posXMax, int posYMin, int posYMax);
	Tmx::Map* getMap() { return _map; }
	std::vector<Brick*> getBrickList() { return _brickList; }
	std::vector<BrickNormal*> getBrickNorList() { return _brickNorList; }
	std::vector<Eagle*> getEagleList() { return _eagleList; }

	//get list brick around entity
	std::vector<Brick*> getBrickListAroundEntity(int posX, int posY);

	bool checkIsGrass(int posX, int posY);
};

