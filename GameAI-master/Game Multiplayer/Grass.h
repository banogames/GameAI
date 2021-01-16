#pragma once
#include "Entity.h"

class Grass :public Entity
{
	Animation* _animation;

public:
	Grass(D3DXVECTOR2 _pos)
	{
		Position = _pos;
		_width = 30;
		_height = 30;
		_animation = new Animation();

		_type = ET_Grass;

		// thêm frame animation
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->tileset, 32, 32 + 32, 64, 64 + 32,
			D3DXVECTOR2(16.f, 16.f)));
	}
	~Grass() {}

	void Draw() {
		if (!IsDeleted) {
			_animation->Draw(Position);
		}
	}
};