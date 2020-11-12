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
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 16, 128, 128 + 16,
			D3DXVECTOR2(8.f, 8.f)));
	}
	~BrickNormal() {}
};

