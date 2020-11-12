#pragma once
#include "Animation.h"
#include "GameGlobal.h"
#include "GameDefine.h"
#include "Astar.h"



class GridTile
{
protected:
	Astar::Vec2 vec;
	Animation* anim;
	D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f);
	TileType type;
	D3DCOLOR color;
	int currentNode = 0;
public:
	GridTile();
	~GridTile() {};
	void SetVec(Astar::Vec2 vec2) { vec = vec2; };
	void SetVec(int x, int y) { vec.reset(x, y); };
	Astar::Vec2 GetVec() { return vec; };
	void Draw();
	TileType GetType() { return type; };
	void SetType(TileType type);
	D3DXVECTOR2 GetPosition() { return position; };
	void SetPosition(D3DXVECTOR2 position) { this->position = position; }
};

