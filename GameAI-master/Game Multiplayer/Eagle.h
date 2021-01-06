#pragma once

#include "Entity.h"

class Eagle:public Entity
{
	Animation* _animation;

public:
	Eagle(D3DXVECTOR2 position, EntityType playerType = ET_EagleNPC)
	{
		_type = playerType;
		Position = position;
		_width = 30;
		_height = 30;
		_animation = new Animation();
		_animation->setScale(D3DXVECTOR2(0.5f, 0.5f));

		// tùy theo network ID mà màu của đại bàng sẽ khác nhau, tương ứng với các người chơi
		switch (playerType)
		{
		case ET_EaglePlayer:
			// màu vàng
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 0, 50, D3DXVECTOR2(25.f, 25.f)));
			break;
		case ET_EagleNPC:	
			// màu xanh
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 0, 50, 50, 100, D3DXVECTOR2(25.f, 25.f)));			
			break;
		//case 2:
		//	// màu trắng
		//_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Eagle, 50, 100, 0, 50, D3DXVECTOR2(25.f, 25.f)));
		//	break;
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

