#pragma once
#include "NPC.h"

class NPCSecurity: public NPC
{
public:
	NPCSecurity(Vec2 posEagle, int rangeSave);
	~NPCSecurity() {};

	Vec2* _posEagle; //tọa độ bảo vệ, luôn random di chuyển xung quang tọa độ bảo vệ
	int _rangeSave; //khoảng cách bảo vệ

	void Init() override;
	void RunRandom() override;
};


