#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "Sprite.h"	
#include "MapReader\Tmx.h.in"
#include "GameGlobal.h"
#include "Brick.h"
#include "BrickNormal.h"
#include "MetalBrick.h"
#include "Water.h"
#include "Boundary.h"
#include "GameCollision.h"
#include "GameDebugDraw.h"
#include "QuadTree.h"

class GameMap
{
	std::vector<Brick*> _brickList;
	std::vector<BrickNormal*> _brickNorList;
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
	Tmx::Map* getMap() { return _map; }
	std::vector<Brick*> getBrickList() { return _brickList; }
	std::vector<BrickNormal*> getBrickNorList() { return _brickNorList; }
};

