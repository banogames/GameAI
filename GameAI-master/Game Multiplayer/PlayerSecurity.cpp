#include "PlayerSecurity.h"

PlayerSecurity::PlayerSecurity(Vec2 posEagle, int rangeSave)
{
	Init();

	_posEagle = new Vec2();
	_posEagle->x = posEagle.x;
	_posEagle->y = posEagle.y;

	_rangeSave = rangeSave;
}

void PlayerSecurity::Init()
{
	NPC::Init();

	_type = ET_Player;

	//chỉ di chuyển quanh 1 vùng được định sẵn
	_speed = 100.0f;
	_heart = rand() % 3 + 2; //2=> 4
	_rangeAttack = rand() % 9 + 6; //6 => 8
	_timeDelayShoot = 4.0f;

	//màu vàng cấp 2
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 76, 76 + 37, 196, 196 + 32, D3DXVECTOR2(18.f, 16.f)));
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 195, 195 + 30, D3DXVECTOR2(18.f, 15.f)));
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 192, 192 + 36, D3DXVECTOR2(16.f, 18.f)));
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 195, 195 + 34, D3DXVECTOR2(15.f, 17.f)));
}

void PlayerSecurity::RunRandom()
{
	Vec2 *pos = new Vec2();
	if (Astar::getInstance()->RandomoPosValidAround(*_posEagle, pos, _rangeSave))
	{
		PathAstar(pos->x, pos->y);
	}
}
