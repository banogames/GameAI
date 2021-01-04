#include "AStarScene.h"
#include "SceneManager.h"

AStarScene::AStarScene()
{
	player = new Player();
	player->Position = D3DXVECTOR2(25.f, 25.f);

	//set grid bat dau
	begin = GridTile();
	begin.SetPosition(D3DXVECTOR2(0.f, 0.f));
	begin.SetVec(0,0);
	begin.SetType(Destination);

	//set grid ket thuc
	destination = GridTile();
	destination.SetPosition(D3DXVECTOR2(0.f, 0.f));
	begin.SetVec(COUNT_X - 1, COUNT_Y -2);
	destination.SetType(Destination);

	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
		{
			map[x][y] = new GridTile();
			map[x][y]->SetVec(x, y);
			map[x][y]->SetPosition(D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP));
		}
	}
}

void AStarScene::OnLeftMouseDown(float x, float y)
{
	//insert, delete
	if (!_isPlayerMoving) 
	{
		int _x = int(x / X_STEP), _y = int(y / Y_STEP);
		SetObstacle(_x, _y);
	}
}

void AStarScene::SetObstacle(int x, int y) 
{
	map[x][y]->SetType(Obstacle);
	astar->SetValue(x, y, VALUE_ASTAR_BRICK);
}

void AStarScene::OnRightMouseDown(float x, float y)
{
	//dung lai
	if (_isPlayerMoving) {
		//giat lui lai
		if (currentNodeIndex < path.size() && currentNodeIndex > 0)
		{
			player->Stop(path[currentNodeIndex - 1]->GetPosition(), path[currentNodeIndex]->GetPosition());
		}
			
		this->currentNodeIndex = 0;
	}

	//move
	ResetScene();
	GridTile* tileBegin = map[begin.GetVec().x][begin.GetVec().y];
	tileBegin->SetType(Empty);
	begin.SetPosition(player->Position);
	begin.SetVec((int)(begin.GetPosition().x / X_STEP), (int)(begin.GetPosition().y / Y_STEP));

	tileBegin = map[begin.GetVec().x][begin.GetVec().y];
	tileBegin->SetType(Begin);

	GridTile* tileDes = map[destination.GetVec().x][destination.GetVec().y];
	tileDes->SetType(Empty);
	destination.SetPosition(D3DXVECTOR2(x, y));
	destination.SetVec((int)(x / X_STEP), (int)(y / Y_STEP));

	tileDes = map[destination.GetVec().x][destination.GetVec().y];
	tileDes->SetType(Destination);

	RunAStar();
	DrawPath();

	_isPlayerMoving = true;
}

void AStarScene::Update(float dt)
{
	GivePlayerOrder();
	player->Update(dt);
}

void AStarScene::ResetScene()
{
	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
		{
			if (map[x][y]->GetType() != Obstacle)
				map[x][y]->SetType(Empty);
		}
	}
	path.clear();
}


void AStarScene::RunAStar()
{
	path.clear();
	Node beginNode;
	beginNode.SetX((int)begin.GetVec().x);
	beginNode.SetY((int)begin.GetVec().y);

	Node destNode;
	destNode.SetX((int)destination.GetVec().x);
	destNode.SetY((int)destination.GetVec().y);

	AstarResuit resuit = astar->findPath(beginNode, destNode);

	//draw closed, opened list
	for (std::vector<Node> ::iterator it = resuit.closeList.begin(); it != resuit.closeList.end(); it = next(it))
	{
		map[it->x][it->y]->SetType(Closed);
	}

	for (std::vector<Node> ::iterator it = resuit.openList.begin(); it != resuit.openList.end(); it = next(it))
	{
		map[it->x][it->y]->SetType(Opened);
	}

	//set path
	for (std::vector<Node> ::iterator it = resuit.path.begin(); it != resuit.path.end(); it = next(it))
	{
		path.push_back(map[it->x][it->y]);
	}
}

void AStarScene::DrawPath()
{
	if (!path.empty())
		for (int index = 0; index < path.size(); index++)
		{
			path[index]->SetType(Path);
		}
	else
		GAMELOG("Path is empty!");
}

void AStarScene::GivePlayerOrder()
{
	if (_isPlayerMoving)
	{
		if (currentNodeIndex < path.size())
		{
			player->Move(path[currentNodeIndex]->GetPosition());
			if (!player->IsMoving()) currentNodeIndex++;
		}
		else
		{
			currentNodeIndex = 0;
			_isPlayerMoving = false;
		}
	}
}

void AStarScene::Draw()
{
	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
		{
			map[x][y]->Draw();
		}
	}

	player->Draw();
}