#include "GameGlobal.h"

#include "GameDefine.h"

HINSTANCE GameGlobal::HInstance = NULL;
HWND GameGlobal::Window = NULL;
LPDIRECT3DDEVICE9 GameGlobal::Device = nullptr;
LPD3DXSPRITE GameGlobal::XSprite = NULL;
bool GameGlobal::IsGameRunning = true;
int GameGlobal::Width = 1200;
int GameGlobal::Height = 650;
