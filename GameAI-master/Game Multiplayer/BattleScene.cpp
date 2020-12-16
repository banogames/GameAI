#include "BattleScene.h"
#include "AStar.h"
#include "GameLog.h"

BattleScene::BattleScene()
{
	_map = new GameMap("Resource files/map4.tmx");

	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
		{
			mapGrid[x][y] = new GridTile();
			mapGrid[x][y]->SetVec(x, y);
			mapGrid[x][y]->SetPosition(D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP));
		}
	}

	_waterBrick = new Water(D3DXVECTOR2(10.0f, 10.0f));

	std::vector<Brick*> bricks = _map->getBrickList();
	for (std::vector<Brick*> ::iterator it = bricks.begin(); it != bricks.end(); it = next(it))
	{
		float posX = (*it)->Position.x;
		float posY = (*it)->Position.y;
		//set Obstacle astar
		Astar::getInstance()->SetValue((posX / X_STEP), (posY / Y_STEP), 1);
	}

	_player = new Player();
	
	//set gia tri astar cua palyer
	Astar::getInstance()->SetValue(_player->Position.x / X_STEP, _player->Position.y / Y_STEP, 2);

	// tạo 3 npcs
	for (int i = 0; i < 5; i++)
	{
		NPC* npc = new NPC();

		Vec2 *pos = new Vec2();
		if (Astar::getInstance()->RandomPosValid(pos))
		{
			npc->SetPosition((pos->x + 0.5)* X_STEP , (pos->y + 0.5)* Y_STEP);
		}
		_npcList.push_back(npc);

		// tạo 1 BULLET cho mỗi NPC
		/*Bullet* bullet = new Bullet();
		_bulletList.push_back(bullet);*/
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

	//_labelPlayer = Label("", 26, 13, D3DXVECTOR2(0, 0));

	
}


void BattleScene::Update(float dt)
{
	for (auto npc : _npcList) // set vận tốc npcs dựa theo direction 
	{
		npc->ApplyVelocity();
	}

	if (!_player->IsDeleted)
	{
		for (NPC *npc : _npcList) // players va chạm npcs
		{
			if (!npc->IsDeleted)
			{
				bool _onColl = false;			
				if (npc->CheckCollision(_player)) 
				{
					if (_onColl == false)
						_onColl = true;
				}
				for (auto npcOther : _npcList)
				{
					if (npc != npcOther) {
						if (npc->CheckCollision(npcOther)) 
						{
							if (_onColl == false)
								_onColl = true;
						}
					}
				}
				npc->SetIsCollision(_onColl);

				_player->CheckCollision(npc);

				npc->CheckPlayerInRange(_player->Position.x / X_STEP, _player->Position.y / Y_STEP);
			}
		}

		for (auto npc : _npcList)
		{
			npc->Update(dt);
		}

		_player->HandleKeyboard(keyboard, dt);

		_player->Update(dt);

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


	//test astar
	//if (keyboard[VK_SPACE]) 
	//{
	//	for (NPC *npc : _npcList) //move ai to player
	//	{
	//		npc->MovoToGridAstar(_player->Position.x / X_STEP, _player->Position.y /Y_STEP);
	//	}
	//}

}

void BattleScene::Draw()
{
	_waterBrick->Draw();

	_map->Draw();

	if (IS_DRAW_PATH_ASTAR) 
	{
		for (NPC *npc : _npcList)
		{
			npc->DrawPath();
		}
	}

	for (auto npc : _npcList)
	{
		npc->Draw();
	}

	_player->Draw();

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
	//_labelPlayer.setPosition(D3DXVECTOR2(850.0f, 100.0f));
	//_labelPlayer.Draw("Player Score: " + to_string(_player->getScore()), D3DCOLOR_XRGB(255, 242, 0));

}

 bool BattleScene::RandomGridTileMove(GridTile *grid)
{
	Vec2 *pos = new Vec2();
	if (Astar::getInstance()->RandomPosValid(pos))
	{
		*grid = *mapGrid[pos->x][pos->y];
		return true;
	}
	return false;
}

