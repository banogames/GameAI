#pragma once
#include "NPC.h"

class NPCSecurity: public NPC
{
public:
	NPCSecurity(Eagle *eagle, int rangeSave);
	~NPCSecurity() {};

	Eagle* _eagle; //tọa độ bảo vệ, luôn random di chuyển xung quang tọa độ bảo vệ
	int _rangeSave; //khoảng cách bảo vệ

	void Init() override;
	void RunRandom() override;
};


