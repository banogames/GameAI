#pragma once

#include "Entity.h"
#include "Animation.h"

class Item : public Entity
{
private:
	const float _existTime = 7.0f;
	float _count_existTime = 0.0f; // đếm
	int _flashingTime = 0; // giúp vẽ nhấp nháy

protected:
	Animation* _animation;

	void BaseInit(D3DXVECTOR2 position)
	{
		Position = position;
		_width = 32;
		_height = 32;
		_animation = new Animation();
	}

public:
	Item() {}
	~Item() { delete _animation; }

	void Update(float _dt) override
	{
		if (IsDeleted)
			return;

		_count_existTime += _dt;
		if (_count_existTime >= _existTime)
		{
			IsDeleted = true;
			_count_existTime = 0.f;
		}
	}

	void Draw() override
	{
		if (IsDeleted)
			return;

		// nếu còn 1.5 giây tồn tại, vẽ nhấp nháy
		if (_existTime - _count_existTime < 1.5f)
		{
			_flashingTime++;
			if (_flashingTime > 5)
			{
				_animation->Draw(Position);
				//flashingTime = 0;
			}
			if (_flashingTime == 10)
			{
				_flashingTime = 0;
			}
		}
		else
		{
			_animation->Draw(Position);
		}
	}
};

