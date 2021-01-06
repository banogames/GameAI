﻿#pragma once

#include "GameGlobal.h"
#include "Animation.h"
#include "SpriteList.h"
#include "GameDefine.h"

class Entity
{
protected:
	int _width = 0,
		_height = 0;
	EntityType _type = ET_Unknown; // nhận diện Entity

public:
	D3DXVECTOR2 Position = D3DXVECTOR2(0.f, 0.f);
	D3DXVECTOR2 Velocity = D3DXVECTOR2(0.f, 0.f);
	bool IsDeleted = false;
	int _heart = 3;

public:
	Entity() {}
	~Entity() {}

	virtual void Update(float dt) {}
	virtual void Draw() {}
	virtual void MakeCollision(Entity* _en) {}	// thực hiện va chạm

	RECT getBound();
	void DecreasedHeart(int _value =1){
		_heart -= _value;
		if (_heart <= 0) IsDeleted = true;
	}

	int getWidth() { return _width; }
	int getHeight() { return _height; }
	EntityType getType() { return _type; }
};
