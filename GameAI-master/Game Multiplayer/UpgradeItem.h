#pragma once

#include "Item.h"

class UpgradeItem : public Item
{
public:
	UpgradeItem(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		_type = ET_UpgradeItem;
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 96 + 32, 32, 32 + 32, 
			D3DXVECTOR2(16.f, 16.f)));
	}
	~UpgradeItem(){}
};

