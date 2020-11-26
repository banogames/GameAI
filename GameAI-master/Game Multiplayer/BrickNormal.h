#pragma once

#include "Brick.h"

class BrickNormal : public Brick
{
public:
	BrickNormal(D3DXVECTOR2 position)
	{
		BaseInit(position);
		_type = ET_NormalBrick;

		// thêm frame animation
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 32, 0, 0 + 32,
			D3DXVECTOR2(16.f, 16.f)));
	}
	~BrickNormal() {}
};

