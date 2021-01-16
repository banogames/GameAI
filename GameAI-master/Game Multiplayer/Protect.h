#pragma once

#include "Entity.h"
#include "SpriteList.h"

class Protect : public Entity
{
	float _existTime = 0.f; // thời gian tồn tại của vụ nổ
	float _count_Exist = 0.f;

	Animation* _animation;
public:
	Protect()
	{
		IsDeleted = true;

		// khởi tạo animation, mỗi frame 0.1 giây
		_animation = new Animation(0.1f);

		// animation
		_existTime =5.f;
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 32, 96, 96 + 64,
			D3DXVECTOR2(8.f, 8.f)));
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 32, 32 + 32, 96, 96 + 64,
			D3DXVECTOR2(16.f, 16.f)));

		/*_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 64, 64, 64 + 64,
			D3DXVECTOR2(32.f, 32.f)));
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 160, 160 + 64, 64, 64 + 64,
			D3DXVECTOR2(32.f, 32.f)));*/

		_count_Exist = _existTime;
	}
	~Protect() {}

	void Update(float dt)
	{
		if (IsDeleted)
			return;

		_animation->Update(dt);

		_count_Exist -= dt;
		if (_count_Exist < 0)
		{
			_count_Exist = _existTime;
			IsDeleted = true;
		}
	}

	void Draw()
	{
		if (IsDeleted)
			return;

		_animation->Draw(Position);
	}

	void Spawn(D3DXVECTOR2 position)
	{
		Position = position;
		_animation->Reset();
		IsDeleted = false;
	}
};

