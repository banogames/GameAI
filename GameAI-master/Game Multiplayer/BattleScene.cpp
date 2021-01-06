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

	//inint eagle
	eagleList = _map->getEagleList();
	for (auto eagle : eagleList)
	{
		switch (eagle->getType())
		{
		case ET_EagleNPC:
			eagleNPCList.push_back(eagle);
			break;
		case ET_EaglePlayer:
			eaglePlayerList.push_back(eagle);
			break;
		default:
			break;
		}
	}

	//tạo other player
	for (int i = 0; i < COUNT_PLAYER_OTHER; i++) 
	{
		Vec2 *pos = new Vec2();
		PlayerOther *playerOther = new PlayerOther();
		NPC* other = static_cast<NPC *>(playerOther);
		if (Astar::getInstance()->RandomPosValid(pos))
		{
			other->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
			Astar::getInstance()->SetValue(pos->x, pos->y, VALUE_ASTAR_NPC);
		}
		_playerList.push_back(other);
		_bulletList.insert(_bulletList.begin(), other->_bulletList.begin(), other->_bulletList.end());
	}
	
	//tạo player security
	int curEagle = -1;
	if (eaglePlayerList.size() > 0)
	{
		for (int i = 0; i < COUNT_PLAYER_SECURITY; i++)
		{
			Vec2 *pos = new Vec2();
			curEagle++;
			if (curEagle >= eaglePlayerList.size())
				curEagle = 0;

			Vec2 vecEagle;
			vecEagle.x = eaglePlayerList.at(curEagle)->Position.x / X_STEP;
			vecEagle.y = eaglePlayerList.at(curEagle)->Position.y / Y_STEP;

			//chạy xung quanh aegle 4 ô
			PlayerSecurity* otherSec = new PlayerSecurity(vecEagle, 4);

			NPC* other = static_cast<NPC *>(otherSec);
			if (Astar::getInstance()->RandomoPosValidAround(vecEagle, pos, 6))
			{
				other->SetPosition(((float)pos->x + 0.5)* X_STEP, ((float)pos->y + 0.5)* Y_STEP);
				Astar::getInstance()->SetValue(pos->x, pos->y, VALUE_ASTAR_NPC);
			}
			_playerList.push_back(other);
			_bulletList.insert(_bulletList.begin(), other->_bulletList.begin(), other->_bulletList.end());
		}
	}

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
	
	//spawn NPC security
	curEagle = -1;
	if (eagleNPCList.size() > 0) 
	{
		for (int i = 0; i < COUNT_NPC_SECURITY; i++)
		{
			Vec2 *pos = new Vec2();
			curEagle++;
			if (curEagle >= eagleNPCList.size())
				curEagle = 0;

			Vec2 vecEagle;
			vecEagle.x = eagleNPCList.at(curEagle)->Position.x / X_STEP;
			vecEagle.y = eagleNPCList.at(curEagle)->Position.y / Y_STEP;

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

		for (auto other : _playerList)
		{
			other->addExplosion(e);
		}

		_player->addExplosion(e);
	}

	// tạo 2 loại Protected Item và Upgrade Item
	_protectItem = new ProtectItem(D3DXVECTOR2(250.0f, 250.0f));
	_protectItem->IsDeleted = true;
	_upgradeItem = new UpgradeItem(D3DXVECTOR2(350.0f, 350.0f));
	_upgradeItem->IsDeleted = true;
	_pointed = new Pointed();

	_labelPlayer = Label("", 40, 20, D3DXVECTOR2(0, 0));
	_labelNPC = Label("", 40, 20, D3DXVECTOR2(0, 0));
}


void BattleScene::Update(float dt)
{
	if (camera)
	{
		camera->Update();
	}

	//for (auto npc : _npcList) // set vận tốc npcs dựa theo direction 
	//{
	//	npc->ApplyVelocity();
	//}

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
					if (npc != npcOther) 
					{
						if (npc->CheckCollision(npcOther)) 
						{
							if (_onColl == false)
								_onColl = true;
						}
					}
				}
				for (auto playerOther : _playerList)
				{
					if (npc->CheckCollision(playerOther))
					{
						if (_onColl == false)
							_onColl = true;
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

				//check eagle
				for (auto eagle : eagleList)
				{
					if (npc->CheckCollision(eagle)) 
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				npc->SetIsCollision(_onColl);

				_player->CheckCollision(npc);


				//quét theo thứ tự: player => player other => player eagle
				bool inRange = false;
				inRange = npc->CheckPlayerInRange(_player->Position.x / X_STEP, _player->Position.y / Y_STEP);

				if (!inRange) {
					for (NPC *other : _playerList)
					{
						if (!other->IsDeleted) {
							if (npc->CheckPlayerInRange(other->Position.x / X_STEP, other->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}	
					}
				}
				
				if (!inRange) 
				{
					for (Eagle* eagle : eaglePlayerList)
					{
						if (!eagle->IsDeleted) 
						{
							if (npc->CheckPlayerInRange(eagle->Position.x / X_STEP, eagle->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}	
					}
				}	
			}
		}

		//other player
		for (NPC *other : _playerList) // players va chạm npcs
		{
			other->Update(dt);
			if (!other->IsDeleted)
			{
				bool _onColl = false;
				if (other->CheckCollision(_player))
				{
					if (_onColl == false)
						_onColl = true;
				}
				for (auto playerOther : _playerList)
				{
					if (other != playerOther) {
						if (other->CheckCollision(playerOther))
						{
							if (_onColl == false)
								_onColl = true;
						}
					}
				}
				for (auto npc : _npcList)
				{
					if (other->CheckCollision(npc))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check va chạm với brick
				for (auto brick : _map->getBrickListAroundEntity(other->Position.x / X_STEP, other->Position.y / Y_STEP))
				{
					if (other->CheckCollision(brick))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				//check eagle
				for (auto eagle : eagleList)
				{
					if (other->CheckCollision(eagle))
					{
						if (_onColl == false)
							_onColl = true;
					}
				}

				other->SetIsCollision(_onColl);

				_player->CheckCollision(other);


				//quét theo thứ tự: npc => npc eagle
				bool inRange = false;
				if (!inRange) 
				{
					for (NPC *npc : _npcList)
					{
						if (!npc->IsDeleted) 
						{
							if (other->CheckPlayerInRange(npc->Position.x / X_STEP, npc->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}
					}
				}
				if (!inRange)
				{
					for (Eagle* eagle : eagleNPCList)
					{
						if (!eagle->IsDeleted)
						{
							if (other->CheckPlayerInRange(eagle->Position.x / X_STEP, eagle->Position.y / Y_STEP))
							{
								inRange = true;
								break;
							}
						}				
					}
				}
			}
		}


		//check brick
		for (auto brick : _map->getBrickListAroundEntity(_player->Position.x / X_STEP, _player->Position.y / Y_STEP))
		{
			_player->CheckCollision(brick);
		}

		//check eagle
		for (auto eagle : eagleList)
		{
			_player->CheckCollision(eagle);
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

	if (IS_DRAW_PATH_ASTAR)
	{
		for (auto other : _playerList)
		{
			other->DrawPath();
		}
	}

	for (auto npc : _npcList)
	{
		npc->Draw();
	}

	for (auto other : _playerList)
	{
		other->Draw();
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

	int eagleNPC =0; //lưu lại để vẽ
	int eaglePlayer=0; //lưu lại để vẽ

	for (auto e : eagleNPCList) 
	{
		if (!e->IsDeleted) eagleNPC += 1;
		e->Draw();
	}

	for (auto e : eaglePlayerList)
	{
		if (!e->IsDeleted) eaglePlayer += 1;
		e->Draw();
	}

	// vẽ điểm ng chơi theo tọa độ của camera => di chuyển cùng camera
	_labelPlayer.setPosition(D3DXVECTOR2(camera->posX - X_MAX / 2 + X_STEP * 0.5f, camera->posY - Y_MAX / 2));
	_labelPlayer.Draw("x "+ to_string(eaglePlayer), D3DCOLOR_XRGB(255, 242, 0));

	//vẽ điểm NPC
	_labelNPC.setPosition(D3DXVECTOR2(camera->posX + X_MAX / 2 - X_STEP * 2.5f, camera->posY - Y_MAX / 2));
	_labelNPC.Draw("x " + to_string(eagleNPC), D3DCOLOR_XRGB(255, 242, 0));
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

