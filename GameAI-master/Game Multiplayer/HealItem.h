#pragma once

#include "Item.h"

class HealItem : public Item
{
public:
	HealItem(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		_type = ET_HealItem;
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 32, 32, 32 + 32,
			D3DXVECTOR2(16.f, 16.f)));
		_heart = 2;
	}
	~HealItem() {}

};

