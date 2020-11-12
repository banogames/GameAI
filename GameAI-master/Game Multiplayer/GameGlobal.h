#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>

class GameGlobal
{
public:
	static HINSTANCE HInstance;
	static HWND Window;
	static LPDIRECT3DDEVICE9 Device;
	static LPD3DXSPRITE XSprite;

	static bool IsGameRunning;

	static int Width;
	static int Height;
};