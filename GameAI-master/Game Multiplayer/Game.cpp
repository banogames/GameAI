#include "Game.h"

#include "GameGlobal.h"
#include "SceneManager.h"
#include "GameLog.h"
//#include "BattleScene.h"
#include "AStarScene.h"
#include <stdio.h>

Game::Game()
{
	//SceneManager::Instance()->ReplaceScene(new BattleScene());
	SceneManager::Instance()->ReplaceScene(new AStarScene());

	InitLoop();
}

void Game::Update(float dt)
{
	SceneManager::Instance()->GetCurrentScene()->Update(dt);
}

void Game::Render()
{
	auto device = GameGlobal::Device;
	auto scene = SceneManager::Instance()->GetCurrentScene();

	device->Clear(0, NULL, D3DCLEAR_TARGET, scene->GetBackcolor(), 0.0f, 0);

	{
		device->BeginScene();

		//bat dau ve
		GameGlobal::XSprite->Begin(D3DXSPRITE_ALPHABLEND);

		//draw here
		scene->Draw();

		//ket thuc ve
		GameGlobal::XSprite->End();

		device->EndScene();
	}

	device->Present(0, 0, 0, 0);
}

void Game::InitLoop()
{
	MSG msg;
	float delta = 0;

	while (GameGlobal::IsGameRunning)
	{
		GameTime::getInstance()->StartCounter();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		delta += GameTime::getInstance()->getCouter();

		if (delta >= 1 / 60.f)
		{
			Update(delta);
			Render();
			delta = 0;
		}
		else
		{
			Sleep((DWORD)(1 / 60.f - delta));
			delta = 1 / 60.f;
		}
	}
}