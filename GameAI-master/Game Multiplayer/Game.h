#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include <dinput.h>
#include "GameTime.h"
#include <thread>

class Game
{
public:
	Game();
	~Game() {}

private:
	void InitLoop(); // vòng lặp game
	void Render(); // vẽ hình
	void Update(float dt); // cập nhật game
};

