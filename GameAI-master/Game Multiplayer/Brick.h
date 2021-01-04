#pragma once

#include "Entity.h"
#include "Animation.h"
#include "SpriteList.h"

class Brick : public Entity
{
protected:
	Animation* _animation;

public:
	Brick() { _heart = 2; }
	~Brick()  { delete _animation; }

	void Draw() {
		if (!IsDeleted) {
			_animation->Draw(Position);
		}	
	}

// các hàm hỗ trợ
protected:
	void BaseInit(D3DXVECTOR2 position)
	{
		Position = position;
		_width = 28;
		_height = 28;
		_animation = new Animation();
	}
};

