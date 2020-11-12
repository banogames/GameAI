#pragma once

#include "Entity.h"

class Eagle:public Entity
{
	Animation* _animation;

public:
	Eagle(D3DXVECTOR2 position, int color = 0)
	{
		_type = ET_Eagle;
		Position = position;
		_width = 50;
		_height = 50;
		_animation = new Animation();

		// tùy theo network ID mà màu của đại bàng sẽ khác nhau, tương ứng với các người chơi
		switch (color)
		{
		case 0:
			// màu vàng
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 1:
			// màu trắng
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case 2:
			// màu xanh
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 50, 100, D3DXVECTOR2(25.f, 25.f)));
			break;
		default:
			// màu đỏ
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 50, 100, D3DXVECTOR2(25.f, 25.f)));
			break;
		}
	}

	void Draw()
	{
		if (IsDeleted) 
			return;

		_animation->Draw(Position);
	}
};

