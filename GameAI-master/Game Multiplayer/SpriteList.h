#pragma once

#include "Sprite.h"

class SpriteList
{
private:
	SpriteList() 
	{
		Box = new Sprite("Resource files/Box.png");
		tileset = new Sprite("Resource files/tileset.png");
		Others = new Sprite("Resource files/Others.png", D3DCOLOR_XRGB(255, 0, 255));
		Eagle = new Sprite("Resource files/Eagle.png", D3DCOLOR_XRGB(255, 0, 255));
		TankAI = new Sprite("Resource files/TankAI.png", D3DCOLOR_XRGB(255, 0, 255));
		Tank = new Sprite("Resource files/Tank.png", D3DCOLOR_XRGB(255, 0, 255));
		Arrow = new Sprite("Resource files/Arrow.png", D3DCOLOR_XRGB(255, 0, 255));
		Point = new Sprite("Resource files/Point.png", D3DCOLOR_XRGB(255, 0, 0));
		All = new Sprite("Resource files/All.png", D3DCOLOR_XRGB(255, 0, 255));
		WhiteTile = new Sprite("Resource files/WhiteTile.png", D3DCOLOR_XRGB(255, 0, 255));
	}
	static SpriteList* _instance;

public:
	static SpriteList* Instance()
	{
		if (_instance == nullptr)
		{
			_instance = new SpriteList();
		}

		return _instance;
	}

	Sprite* Arrow;
	Sprite* Box;
	Sprite* Eagle;
	Sprite* MapObject;
	Sprite* Others;
	Sprite* Others2;
	Sprite* PlusBox;
	Sprite* Point;
	Sprite* Tank;
	Sprite* TankAI;
	Sprite* tileset;
	Sprite* All;
	Sprite* WhiteTile;
};

