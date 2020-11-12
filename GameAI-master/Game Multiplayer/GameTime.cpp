#include "GameTime.h"

GameTime* GameTime::_instance = NULL;

GameTime::GameTime() {}
GameTime::~GameTime() {}

GameTime* GameTime::getInstance()
{
	if (!_instance)
		_instance = new GameTime();

	return _instance;
}

void GameTime::StartCounter()
{
	if (!QueryPerformanceFrequency(&_clockRate))
	{
		return;
	}

	QueryPerformanceCounter(&_startTime);

}

float GameTime::getCouter()
{
	QueryPerformanceCounter(&_endTime);
	_delta.QuadPart = _endTime.QuadPart - _startTime.QuadPart;

	return ((float)_delta.QuadPart / _clockRate.QuadPart);
}