#include "BattleScene.h"
#include "AStar.h"
#include "GameLog.h"
#include "GameDefine.h"

void BattleScene::InitCamera(GraphicsDevice* gDevice) 
{
	camera = new Camera(gDevice, X_MAX, Y_MAX, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	camera->Follow(_player);
	//camera->Follow(_npcList.at(0));
}

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
		Astar::getInstance()->SetValue((posX / X_STEP), (posY / Y_STEP), VALUE_ASTAR_BRICK);
	}

	_player = new Player();
	_bulletList.insert(_bulletList.begin(),_player->_bulletList.begin(), _player->_bulletList.end());

	
	//set gia tri astar cua palyer
	Astar::getInstance()->SetValue(_player->Position.x / X_STEP, _player->Position.y / Y_STEP, VALUE_ASTAR_PLAYER);


	// tạo npcs
	for (int i = 0; i < COUNT_NPC_FAST; i++)
	{
		Vec2 *pos = new Vec2();
		NPCFast *npcFast = new NPCFast();
		NPC* npc = static_cast<NPC *>(npcFast);
		if (Astar::getInstance()->RandomPosValid(pos))
		{
			npc->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
			Astar::getInstance()->SetValue(pos->x, pos->y, VALUE_ASTAR_NPC);
		}
		_npcList.push_back(npc);
		_bulletList.insert(_bulletList.begin(), npc->_bulletList.begin(), npc->_bulletList.end());
	}

	for (int i = 0; i < COUNT_NPC_TANK; i++)
	{
		Vec2 *pos = new Vec2();
		NPCTank *npcTank = new NPCTank();
		NPC* npc = static_cast<NPC *>(npcTank);
		if (Astar::getInstance()->RandomPosValid(pos))
		{
			npc->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
			Astar::getInstance()->SetValue(pos->x, pos->y, VALUE_ASTAR_NPC);
		}
		_npcList.push_back(npc);
		_bulletList.insert(_bulletList.begin(), npc->_bulletList.begin(), npc->_bulletList.end());
	}

	int curEagle = -1;
	for (int i = 0; i < COUNT_NPC_SECURITY; i++)
	{
		Vec2 *pos = new Vec2();
		

		//lấy vị trí của eagle tiếp => cho nó bảo vệ eagle đó
		vector<Eagle*> eagleList = _map->getEagleList();
		curEagle++;
		if (curEagle >= eagleList.size())
			curEagle = 0;

		Vec2 vecEagle;
		vecEagle.x = eagleList.at(curEagle)->Position.x / X_STEP;
		vecEagle.y = eagleList.at(curEagle)->Position.y / Y_STEP;

		//chạy xung quanh aegle 4 ô
		NPCSecurity* npcSec = new NPCSecurity(vecEagle, 4);

		NPC* npc = static_cast<NPC *>(npcSec);
		if (Astar::getInstance()->RandomoPosValidAround(vecEagle, pos, 6))
		{
			npc->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
			Astar::getInstance()->SetValue(pos->x, pos->y, VALUE_ASTAR_NPC);
		}
		_npcList.push_back(npc);
		_bulletList.insert(_bulletList.begin(), npc->_bulletList.begin(), npc->_bulletList.end());
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
	if (camera)
	{
		camera->Update();
	}

	for (auto npc : _npcList) // set vận tốc npcs dựa theo direction 
	{
		npc->ApplyVelocity();
	}

	if (!_player->IsDeleted)
	{
		for (NPC *npc : _npcList) // players va chạm npcs
		{
			npc->Update(dt);
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

				//check va chạm với brick
				for (auto brick : _map->getBrickListAroundEntity(npc->Position.x / X_STEP, npc->Position.y / Y_STEP))
				{
					if (npc->CheckCollision(brick))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				npc->SetIsCollision(_onColl);

				_player->CheckCollision(npc);

				npc->CheckPlayerInRange(_player->Position.x / X_STEP, _player->Position.y / Y_STEP);

			}
		}

		//check brick
		for (auto brick : _map->getBrickListAroundEntity(_player->Position.x / X_STEP, _player->Position.y / Y_STEP))
		{
			_player->CheckCollision(brick);
		}

		_player->HandleKeyboard(keyboard, dt);

		_player->Update(dt);

	}


	for (auto bullet : _bulletList)
	{
		bullet->Update(dt);

		//check va chạm brick xung quang
		for (auto brick : _map->getBrickListAroundEntity(bullet->Position.x / X_STEP, bullet->Position.y / Y_STEP))
		{
			bullet->CheckCollision(brick);
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
	if (keyboard[VK_END]) 
	{
		for (NPC *npc : _npcList) //move ai to player
		{
			npc->MoveGridAstar(_player->Position.x / X_STEP, _player->Position.y /Y_STEP);
		}
	}

}

void BattleScene::Draw()
{
	if (camera)
	{
		camera->Draw();
		_map->DrawInCamera((camera->posX - camera->width/2)/X_STEP, (camera->posX + camera->width / 2)/X_STEP, 
			(camera->posY - camera->height / 2)/Y_STEP, (camera->posY + camera->height / 2)/Y_STEP);
	}

	//chỉ vẽ các objec ở trong camera

	//_waterBrick->Draw();

	//_map->Draw();

	// tránh đường đi nằm phía trên NPC
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

