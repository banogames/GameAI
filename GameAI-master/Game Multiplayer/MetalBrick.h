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
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 0, 0 + 16, 144, 144 + 16,
			D3DXVECTOR2(8.f, 8.f)));
	}

	~MetalBrick() {}
};

