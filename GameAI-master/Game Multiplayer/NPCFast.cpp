#include "NPCFast.h"

NPCFast::NPCFast() 
{
	Init();
}

void NPCFast::Init() 
{
	NPC::Init();

	//máu ít, tốc độ nhanh, vùng bắn rộng
	_speed = 150.0f;
	_heart = rand() % 3 + 4; //4=> 6
	_rangeAttack = rand() % 5 + 10; //10 => 14
	_timeDelayShoot = 2.0f;

	//màu trắng
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 4, 4 + 32, D3DXVECTOR2(16.f, 16.f)));
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
}