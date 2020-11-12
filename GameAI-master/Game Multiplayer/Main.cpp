#include <d3d9.h>
#include <d3dx9.h>

#include <Windows.h>
#include <iostream>
#include <sstream>
#include <dinput.h>
#include <windowsx.h>
#include "GameGlobal.h"
#include "GameTime.h"
#include "SceneManager.h"
#include "Game.h"

using namespace std;

#define WIN_NAME L"Game AI"
#define WIN_TITLE L"Game AI"
#define FPS 60
#define KEYBOARD_BUFFERD_SIZE 1024

int InitWindow(int cmdShow);
int InitDevice();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LPDIRECT3D9             _direct3D9;
LPD3DXSPRITE            _spriteHandler;
PDIRECT3D9              _d3d;
LPDIRECT3DDEVICE9       _device;
HINSTANCE               _hInstance;
int                     _cmdShow;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	_hInstance = hInstance;
	InitWindow(cmdShow);

	return 0;
}

int InitWindow(int cmdShow)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = _hInstance;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WIN_NAME;
	wc.hIconSm = NULL;
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(
		WIN_NAME,
		WIN_NAME,
		WS_OVERLAPPEDWINDOW,
		116,
		40,
		GameGlobal::Width,
		GameGlobal::Height,
		NULL,
		NULL,
		_hInstance,
		NULL);

	GameGlobal::HInstance = _hInstance;
	GameGlobal::Window = hWnd;

	ShowWindow(hWnd, cmdShow);
	UpdateWindow(hWnd);

	if (InitDevice())
	{
		Game *game = new Game();
	}

	return 0;
}

int InitDevice()
{
	_d3d = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = GameGlobal::Width;
	d3dpp.BackBufferHeight = GameGlobal::Height;

	HRESULT dvresult = _d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GameGlobal::Window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&_device);
	GameGlobal::Device = _device;

	D3DXCreateSprite(GameGlobal::Device, &_spriteHandler);
	GameGlobal::XSprite = _spriteHandler;

	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		GameGlobal::IsGameRunning = false;	
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		SceneManager::Instance()->GetCurrentScene()->OnKeyDown(wParam);
		break;

	case WM_KEYUP:
		SceneManager::Instance()->GetCurrentScene()->OnKeyUp(wParam);
		break;

	case WM_LBUTTONDOWN:
		SceneManager::Instance()->GetCurrentScene()->OnLeftMouseDown((float)GET_X_LPARAM(lParam),
			(float)GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONUP:
		SceneManager::Instance()->GetCurrentScene()->OnLeftMouseUp((float)GET_X_LPARAM(lParam),
			(float)GET_Y_LPARAM(lParam));
		break;
		
	case WM_RBUTTONDOWN:
		SceneManager::Instance()->GetCurrentScene()->OnRightMouseDown((float)GET_X_LPARAM(lParam),
			(float)GET_Y_LPARAM(lParam));
	case WM_RBUTTONUP:
		SceneManager::Instance()->GetCurrentScene()->OnRightMouseUp((float)GET_X_LPARAM(lParam),
			(float)GET_Y_LPARAM(lParam));
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}