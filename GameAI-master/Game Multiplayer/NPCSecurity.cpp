#include "NPCSecurity.h"

NPCSecurity::NPCSecurity(Vec2 posEagle, int rangeSave)
{
	Init();

	_posEagle = new Vec2();
	_posEagle->x = posEagle.x;
	_posEagle->y = posEagle.y;

	_rangeSave = rangeSave;
}

void NPCSecurity::Init() 
{
	NPC::Init();

	_type = ET_NPC;

	//chỉ di chuyển quanh 1 vùng được định sẵn
	_speed = 100.0f;
	_heart = rand() % 3 + 2; //2=> 4
	_rangeAttack = rand() % 9 + 6; //6 => 8
	_timeDelayShoot = 4.0f;

	//màu đỏ
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 312, 312 + 32, D3DXVECTOR2(16.f, 16.f)));
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
}

void NPCSecurity::RunRandom() 
{
	Vec2 *pos = new Vec2();
	if (Astar::getInstance()->RandomoPosValidAround(*_posEagle, pos, _rangeSave))
	{
		PathAstar(pos->x, pos->y);
	}
}
