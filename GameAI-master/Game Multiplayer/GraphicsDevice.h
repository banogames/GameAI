#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#include "GameGlobal.h"

class GraphicsDevice
{
public:
	LPDIRECT3D9             _direct3D9;
	PDIRECT3D9              _d3d;
	LPDIRECT3DDEVICE9       _device;
	LPD3DXSPRITE            _spriteHandler;

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
};

