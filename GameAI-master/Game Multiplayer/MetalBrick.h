#pragma once

#include "Brick.h"

class MetalBrick : public Brick
{
public:
	MetalBrick(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		_type = ET_MetalBrick;
		
		// thêm frame animation
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 32, 32, 32 + 32,
			D3DXVECTOR2(16.f, 16.f)));
	}

	~MetalBrick() {}
};

