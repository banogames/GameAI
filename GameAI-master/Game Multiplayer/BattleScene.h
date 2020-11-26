#pragma once
#include <array>

#include "GridTile.h"
#

#include "Scene.h"
#include "GameMap.h"
#include "Player.h"
#include "Bullet.h"
#include "NPC.h"
#include "Explosion.h"
#include "ProtectItem.h"
#include "UpgradeItem.h"
#include "Pointed.h"
#include "Water.h"

#include "Astar.h"

class BattleScene : public Scene
{
	GameMap* _map;
	Player* _player;
	vector<Bullet*> _bulletList;
	vector<NPC*> _npcList;
	vector<Explosion*> _smallExList;
	vector<Explosion*> _bigExplosionList;
	ProtectItem* _protectItem;
	UpgradeItem* _upgradeItem;
	Pointed* _pointed;

	/*Label _labelPlayer;
	Label _labelCountTime;*/

	Water* _waterBrick;

public:
	std::array<std::array<GridTile*, COUNT_Y>, COUNT_X> mapGrid;

	BattleScene();
	~BattleScene() {}
	void Update(float _dt) override;
	void Draw() override;
	bool RandomGridTileMove(GridTile *grid);
};

