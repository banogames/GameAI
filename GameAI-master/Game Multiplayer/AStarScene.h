#include <array>
#include <vector>
#include "Scene.h"

#include "GameDefine.h"
#include "SpriteList.h"
#include "GameGlobal.h"
#include "GameLog.h"
#include "Player.h"
#include "GridTile.h"

#include "Astar.h"

class AStarScene : public Scene
{
	Astar* astar;
	Player* player;

	GridTile begin, destination;
	array<array<GridTile*, COUNT_Y>, COUNT_X> map;
	vector<GridTile*> path;

	int mapAStar[COUNT_X][COUNT_Y];
	int currentNodeIndex = 0;
	bool _isPlayerMoving = false;
	bool _isMouseActive = true;
	int mode = 1;

	void ResetScene();
	void RunAStar();
	void DrawPath();
	void GivePlayerOrder();
public:
	AStarScene();
	~AStarScene() {}
	void Update(float dt) override;
	void Draw() override;

	void FindPathAstar();
	void OnLeftMouseDown(float x, float y) override;
	void OnRightMouseDown(float x, float  y) override;
};



