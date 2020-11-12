#pragma once

#include "Entity.h"
#include "Animation.h"

namespace GIVEPOINT {
	const RECT ONE_HUNDRED = RECT{ 5,0, 32,14 };
	const RECT TWO_HUNDRED = RECT{ 37,0, 65,14 };
	const RECT THREE_HUNDRED = RECT{ 70,0, 98,14 };
	const RECT FOUR_HUNDRED = RECT{ 103,0,131,14 };
	const RECT FIVE_HUNDRED = RECT{ 136,0,164,14 };
}
class Pointed : Entity
{
	Animation* OneHundredAnimation;
	Animation* TwoHundredAnimation;
	float count_Exist = 2.f;

public:
	Pointed()
	{
		OneHundredAnimation = new Animation();
		OneHundredAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Point, 5, 32, 0, 14,
			D3DXVECTOR2(14.f, 7.f)));

		TwoHundredAnimation = new Animation();
		TwoHundredAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->PlusBox, 37, 65, 0, 14,
			D3DXVECTOR2(14.f, 7.f)));

		Position = D3DXVECTOR2(250, 250);
	}
	~Pointed(){}

	void Update(float dt)
	{
		if (IsDeleted)
			return;

		count_Exist -= dt;
		if (count_Exist < 0)
		{
			count_Exist = 1.f;
			IsDeleted = true;
		}
	}

	void Draw()
	{
		if (IsDeleted)
			return;

		OneHundredAnimation->Draw(Position);
	}
};

