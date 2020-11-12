#pragma once

#include "Item.h"

class ProtectItem : public Item
{
public:
	ProtectItem(D3DXVECTOR2 _pos)
	{
		BaseInit(_pos);
		_type = ET_ProtectItem;
		_animation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 32, 32, 64, 
			D3DXVECTOR2(16.f, 16.f)));

	}
	~ProtectItem() {}
};

