#pragma once
#include <Windows.h>

class GameTime
{
public:
	void StartCounter();
	float getCouter();
	static GameTime* getInstance();
	~GameTime();

private:
	GameTime();
	LARGE_INTEGER _startTime, _endTime, _delta, _clockRate;
	static GameTime *_instance;
};

