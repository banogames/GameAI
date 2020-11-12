#pragma once

#include "Brick.h"

class Boundary : public Brick
{
public:
	Boundary(D3DXVECTOR2 position)
	{
		BaseInit(position);
		_type = ET_Boundary;

		// thêm frame animation
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 32, 32 + 16, 144, 144 + 16, D3DXVECTOR2(8.f, 8.f)));
	}
	~Boundary() {}
};

