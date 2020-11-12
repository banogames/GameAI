#pragma once

#include "Entity.h"
#include "SpriteList.h"

class Explosion : public Entity
{	
	float _existTime = 0.f; // thời gian tồn tại của vụ nổ
	float _count_Exist = 0.f;

	Animation* _animation;
public:
	Explosion(bool isBig)
	{
		IsDeleted = true;

		// khởi tạo animation, mỗi frame 0.1 giây
		_animation = new Animation(0.1f);
		
		// animation nổ bé
		if (!isBig)
		{ 
			_existTime = 0.28f;
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 32, 64, 64 + 32,
				D3DXVECTOR2(16.f, 16.f)));
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 32, 32 + 32, 64, 64 + 32,
				D3DXVECTOR2(16.f, 16.f)));
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 64, 64 + 32, 64, 64 + 32,
				D3DXVECTOR2(16.f, 16.f)));
		}
		// animation nổ lớn
		else
		{
			_existTime = 0.18f;
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 64, 64, 64 + 64,
				D3DXVECTOR2(32.f, 32.f)));
			_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 160, 160 + 64, 64, 64 + 64,
				D3DXVECTOR2(32.f, 32.f)));
		}

		_count_Exist = _existTime;
	}
	~Explosion() {}

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

