#pragma once
#include "NPC.h"

class NPCTank : public NPC
{
public:
	NPCTank();
	~NPCTank() {};
	void Init() override;
};

