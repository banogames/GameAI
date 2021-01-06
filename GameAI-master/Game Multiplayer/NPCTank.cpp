#include "NPCTank.h"

NPCTank::NPCTank()
{
	Init();
}

void NPCTank::Init()
{
	NPC::Init();

	_type = ET_NPC;

	//máu nhiều, tốc độ chậm, vùng bắn hẹp
	_speed = 80.0f;
	_heart = rand() % 3 + 8; //8=> 10
	_rangeAttack = rand() % 3 + 4; //4 => 6
	_timeDelayShoot = 6.0f;

	//màu xanh
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 312, 312 + 32, D3DXVECTOR2(16.f, 16.f)));
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
}