#include "BattleScene.h"
#include "AStar.h"
#include "GameLog.h"

BattleScene::BattleScene()
{
	_map = new GameMap("Resource files/map.tmx");

	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
		{
			map[x][y] = new GridTile();
			map[x][y]->SetVec(x, y);
			map[x][y]->SetPosition(D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP));

			//Change value of obstacles node.
			/*if (obstaclesNode.count(x + y * (WIDTH / X_STEP)))
				map[x][y]->SetType(Obstacle);*/
		}
	}

	_player = new Player();
	// tạo 4 bullet cho player
	for (int i = 0; i < 6; i++)
	{
		Bullet* bullet = new Bullet();
		_bulletList.push_back(bullet);
	}

	// tạo 3 npcs
	for (int i = 0; i < 3; i++)
	{
		NPC* npc = new NPC();
		_npcList.push_back(npc);

		// tạo 1 BULLET cho mỗi NPC
		Bullet* bullet = new Bullet();
		_bulletList.push_back(bullet);
	}

	// tạo 10 small explosion
	for (int i = 0; i < 10; i++)
	{
		Explosion* e = new Explosion(false);
		_smallExList.push_back(e);

		for (auto bullet : _bulletList)
		{
			bullet->addExpolostion(e);
		}
	}

	// tạo 5 big explosion
	for (int i = 0; i < 5; i++)
	{
		Explosion* e = new Explosion(true);
		_bigExplosionList.push_back(e);

		for (auto npc : _npcList)
		{
			npc->addExplosion(e);
		}

		_player->addExplosion(e);
	}

	// tạo 2 loại Protected Item và Upgrade Item
	_protectItem = new ProtectItem(D3DXVECTOR2(250.0f, 250.0f));
	_protectItem->IsDeleted = true;
	_upgradeItem = new UpgradeItem(D3DXVECTOR2(350.0f, 350.0f));
	_upgradeItem->IsDeleted = true;
	_pointed = new Pointed();

	_labelPlayer = Label("", 26, 13, D3DXVECTOR2(0, 0));

	_waterBrick = new Water(D3DXVECTOR2(10.0f, 10.0f));
}

void BattleScene::MovePlayer()
{

}

void BattleScene::Update(float dt)
{
	_player->HandleKeyboard(keyboard, dt);
	//_player->Move();

	for (auto npc : _npcList) // set vận tốc npcs dựa theo direction 
	{
		npc->ApplyVelocity();
	}
	
	for (auto bullet : _bulletList) // set vận tốc bullets dựa theo direction
	{
		bullet->ApplyVelocity();
	}	

	_player->Update(dt);
	for (auto npc : _npcList)
	{
		npc->Update(dt);
	}

	if (!_player->IsDeleted)
	{
		for (auto npc : _npcList) // players va chạm npcs
		{
			if (!npc->IsDeleted)
			{
				_player->CheckCollision(npc);
				npc->CheckCollision(_player);
			}
		}
	}

	// sau khi có tọa độ mới tại frame này thì check va chạm với gạch
	for (auto brick : _map->getBrickList())
	{
		if (!brick->IsDeleted)
		{
			_player->CheckCollision(brick);

			for (auto npc : _npcList) // npcs va chạm bricks
			{
				npc->CheckCollision(brick);
			}
		}
	}

	// update các object còn lại
	{
		for (auto bullet : _bulletList)
		{
			bullet->Update(dt);
		}
		for (auto explosion : _smallExList)
		{
			explosion->Update(dt);
		}
		for (auto explosion : _bigExplosionList)
		{
			explosion->Update(dt);
		}
		_protectItem->Update(dt);
		_upgradeItem->Update(dt);
		_pointed->Update(dt);
	}
}

void BattleScene::Draw()
{
	_player->Draw();

	for (auto npc : _npcList)
	{
		npc->Draw();
	}

	_waterBrick->Draw();

	_map->Draw();

	for (auto bullet : _bulletList)
	{
		bullet->Draw();
	}

	for (auto e : _smallExList)
	{
		e->Draw();
	}

	for (auto e : _bigExplosionList)
	{
		e->Draw();
	}

	_protectItem->Draw();
	_upgradeItem->Draw();
	_player->DrawArrow();
	_pointed->Draw();

	// vẽ điểm ng chơi
	_labelPlayer.setPosition(D3DXVECTOR2(850.0f, 100.0f));
	_labelPlayer.Draw("Player Score: " + to_string(_player->getScore()), D3DCOLOR_XRGB(255, 242, 0));
}
