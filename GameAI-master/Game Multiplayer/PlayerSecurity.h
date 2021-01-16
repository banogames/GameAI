#pragma once
#include "NPC.h"

class PlayerSecurity: public NPC
{
public:
	PlayerSecurity(Eagle *eagle, int rangeSave);
	~PlayerSecurity() {};

	Eagle *_eagle; //tọa độ bảo vệ, luôn random di chuyển xung quang tọa độ bảo vệ
	int _rangeSave; //khoảng cách bảo vệ

	void Init() override;
	void RunRandom() override;
};

