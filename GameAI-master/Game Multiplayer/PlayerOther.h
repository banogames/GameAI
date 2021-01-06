#pragma once

#include "NPC.h"

class PlayerOther : public NPC
{
public:
	PlayerOther();
	~PlayerOther() {};
	void Init() override;
};

