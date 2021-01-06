#pragma once
#include "NPC.h"

class PlayerSecurity: public NPC
{
public:
	PlayerSecurity(Vec2 posEagle, int rangeSave);
	~PlayerSecurity() {};

	Vec2* _posEagle; //tọa độ bảo vệ, luôn random di chuyển xung quang tọa độ bảo vệ
	int _rangeSave; //khoảng cách bảo vệ

	void Init() override;
	void RunRandom() override;
};

