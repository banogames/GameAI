#pragma once

#include "Brick.h"

class Water : public Brick
{
public:
	Water(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		_type = ET_Water;

		// thêm frame animation
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 32, 32 + 32, 96, 96 + 32,
			D3DXVECTOR2(16.f, 16.f)));
	}
	~Water() {}
};

