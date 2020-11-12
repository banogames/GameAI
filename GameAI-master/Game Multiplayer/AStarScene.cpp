#include "AStarScene.h"
#include "SceneManager.h"

AStarScene::AStarScene()
{
	astar = new Astar();

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

			//parse map astar (0)
			mapAStar[x][y] = 0;
		}
	}
}

void AStarScene::OnLeftMouseDown(float x, float y)
{
	//insert, delete
	if (!_isPlayerMoving) 
	{
		int _x = int(x) / X_STEP, _y = int(y) / Y_STEP;
		map[_x][_y]->SetType(Obstacle);
		mapAStar[_x][_y] = 1;
	}
	
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
	tileBegin->SetType(Begin);

	GridTile* tileDes = map[destination.GetVec().x][destination.GetVec().y];
	tileDes->SetType(Empty);
	destination.SetPosition(D3DXVECTOR2(x, y));
	destination.SetVec((int)(x / X_STEP), (int)(y / Y_STEP));
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


void AStarScene::FindPathAstar() 
{
#pragma region v1
	//AStar::Params param;
	//param.width = COUNT_X;
	//param.height = COUNT_Y;
	//param.corner = false; // 4 chieu
	//param.start = begin.GetVec();
	//param.end = destination.GetVec();
	//param.can_pass = [&](const AStar::Vec2 &vec)->bool
	//{
	//	return mapAStar[vec.y][vec.x] == 0;
	//};

	//BlockAllocator allocator;
	//int countBlock = 0;
	//for (int x = 0; x < COUNT_X; x++)
	//{
	//	for (int y = 0; y < COUNT_Y; y++)
	//	{
	//		if (map[x][y]->GetType() == Obstacle)
	//			countBlock++;
	//	}
	//}
	//allocator.allocate(countBlock);

	//astar->set_block_allocator(&allocator);
	//auto path = astar->find(param);

	//return path;
#pragma endregion

}

void AStarScene::RunAStar()
{
#pragma region v1
	//	path.clear();
//	AStar::Node beginNode;
//	beginNode.vec.reset(begin.GetVec().x, begin.GetVec().y);
//
//	AStar::Node destNode;
//	destNode.vec.reset(destination.GetVec().x, destination.GetVec().y);
//
//	std::vector<AStar::Vec2> pathVec2 = FindPathAstar();
//
//	for (std::vector<AStar::Vec2>::iterator it = pathVec2.begin(); it != pathVec2.end(); it = std::next(it))
//	{
//		path.push_back(map[it->x][it->y]);
//	}
//
//	//draw close list
//	std::vector<AStar::Vec2> closeVec = astar->get_closed_vec();
//	for (std::vector<AStar::Vec2>::iterator it = closeVec.begin(); it != closeVec.end(); it = std::next(it))
//	{
//		map[it->x][it->y]->SetType(Closed);
//	}
//
//	//draw opened list
//	std::vector<AStar::Vec2> openVec = astar->get_opened_vec();
//	for (std::vector<AStar::Vec2>::iterator it = openVec.begin(); it != openVec.end(); it = std::next(it))
//	{
//		map[it->x][it->y]->SetType(Opened);
//	}
//
//	//clear
//	this->astar->clear();
#pragma endregion

	path.clear();
	Astar::Node beginNode;
	beginNode.vec.reset((int)begin.GetVec().x, (int)begin.GetVec().y);

	Astar::Node destNode;
	destNode.vec.reset((int)destination.GetVec().x, (int)destination.GetVec().y);

	array<array<Astar::Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> mapAstar;

	astar->makePath(mapAstar, destNode);

	vector<Astar::Node> pathNode = astar->aStar(beginNode, destNode);

	for (vector<Astar::Node> ::iterator it = pathNode.begin(); it != pathNode.end(); it = next(it))
	{
		path.push_back(map[it->vec.x][it->vec.y]);
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