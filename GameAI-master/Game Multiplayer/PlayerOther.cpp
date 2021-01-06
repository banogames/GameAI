#include "PlayerOther.h"

PlayerOther::PlayerOther()
{
	Init();
}

void PlayerOther::Init()
{
	NPC::Init();

	_type = ET_Player;

	//máu ít, tốc độ nhanh, vùng bắn rộng
	_speed = 150.0f;
	_heart = rand() % 3 + 4; 
	_rangeAttack = rand() % 3 + 3; 
	_timeDelayShoot = 2.0f;

	//màu vàng cấp 1
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 4, 4 + 32, D3DXVECTOR2(16.f, 16.f)));
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
}
