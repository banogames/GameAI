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
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 64, 64 + 32, 64, 64 + 32, 
			D3DXVECTOR2(16.f, 16.f)));
	}
	~Boundary() {}
};

