#pragma once
#include <array>

#include "GridTile.h"


#include "Scene.h"
#include "GameMap.h"
#include "Player.h"
#include "Bullet.h"
#include "NPC.h"
#include "NPCSecurity.h"
#include "NPCFast.h"
#include "NPCTank.h"
#include "PlayerOther.h"
#include "PlayerSecurity.h"
#include "Explosion.h"
#include "ProtectItem.h"
#include "UpgradeItem.h"
#include "HealItem.h"
//#include "Pointed.h"
#include "Water.h"
#include "Camera.h"

#include "Astar.h"

class BattleScene : public Scene
{
	Camera* camera;

	GameMap* _map;
	Player* _player;

	vector<NPC*> _npcList;
	vector<NPC*> _playerList;

	vector<Bullet*> _bulletList;
	vector<Explosion*> _smallExList;
	vector<Explosion*> _bigExplosionList;

	vector<Eagle*> eagleList;
	vector<Eagle*> eagleNPCList;
	vector<Eagle*> eaglePlayerList;

	float _timeSpawnProtect;
	float _timeSpawnHeal;
	vector<ProtectItem*> _protectItemList;
	vector<HealItem*> _healItemList;
	//Pointed* _pointed;

	Label _labelPlayer;
	Label _labelNPC;
	Label _labelGame;
	//Label _labelCountTime;

	Water* _waterBrick;

public:
	void InitCamera(GraphicsDevice* gDevice);

	std::array<std::array<GridTile*, COUNT_Y>, COUNT_X> mapGrid;

	BattleScene();
	~BattleScene() {}
	void Update(float _dt) override;
	void Draw() override;
	bool RandomGridTileMove(GridTile *grid);
};

