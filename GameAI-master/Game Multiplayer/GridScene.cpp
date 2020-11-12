#include "GridScene.h"
#include "SceneManager.h"

GridScene::GridScene()
{
	player = new Player();
	player->Position = D3DXVECTOR2(25.f, 25.f);

	begin = GridTile();
	begin.SetPosition(D3DXVECTOR2(0.f, 0.f));	
	begin.SetX(0);
	begin.SetY(0);
	begin.SetType(Destination);

	destination = GridTile();
	destination.SetPosition(D3DXVECTOR2(0.f, 0.f));
	destination.SetX(WIDTH / X_STEP - 1);
	destination.SetY(HEIGHT / Y_STEP - 1);
	destination.SetType(Destination);

	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{			
			map[x][y] = new GridTile();
			map[x][y]->x = x;
			map[x][y]->y = y;
			map[x][y]->position = D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP);

			//Change value of obstacles node.
			if (obstaclesNode.count(x + y * (WIDTH / X_STEP)))
				map[x][y]->SetType(Obstacle);
		}
	}
}

void GridScene::Update(float dt)
{
	if (keyboard[0x31]) // 1 -> normal mode, right click to set destination.
	{
		mode = 1;
		GAMELOG("Mode: %d", mode);
		keyboard[0x31] = false;
	}
	else if (keyboard[0x32]) // 2 -> auto mode, right click will auto find path and move tank to destination.
	{
		mode = 2;
		GAMELOG("Mode: %d", mode);
		keyboard[0x32] = false;
	}
	else if (keyboard[0x33]) // 3 -> editor mode, left click will set obstacle, right click will remove obstacle.
	{
		mode = 3;
		GAMELOG("Mode: %d", mode);
		keyboard[0x33] = false;
	}

	if (keyboard[0x51]) // Q
	{
		RunAStar();		
		keyboard[0x51] = false;
	}
	else if (keyboard[0x57]) // W
	{
		DrawPath();
		keyboard[0x57] = false;
	}
	else if (keyboard[0x45]) // E
	{
		// Move player.
		_isPlayerMoving = true;		
		keyboard[0x45] = false;
	}
	else if (keyboard[0x52]) // R
	{
		ResetScene();
	}
	else if (keyboard[0x54]) // T
	{
		obstaclesNode.clear();
		for (int x = 0; x < (WIDTH / X_STEP); x++)
		{
			for (int y = 0; y < (HEIGHT / Y_STEP); y++)
			{
				if (map[x][y]->type == Obstacle)
					map[x][y]->SetType(Empty);
			}
		}
		keyboard[0x54] = false;
	}
	else if (keyboard[0x46]) // F
	{
		_isMouseActive = !_isMouseActive;
		if (_isMouseActive)
			GAMELOG("Mouse is actived!");
		else
			GAMELOG("Mouse is deactived!");
		keyboard[0x46] = false;
	}	
	
	GivePlayerOrder();
	player->Update(dt);
}

void GridScene::OnLeftMouseDown(float x, float y)
{
	if (!_isMouseActive) return;

	if (mode != 3)
	{
		GAMELOG("(%d, %d)", ((int(x)) / 50), ((int(y)) / 50));
	}
	else
	{
		int _x = int(x) / 50, _y = int(y) / 50;
		int id = _x + _y * (WIDTH / X_STEP);
		obstaclesNode.insert(id);
		map[_x][_y]->SetType(Obstacle);
	}
}

void GridScene::OnRightMouseDown(float x, float y)
{
	if (!_isMouseActive) return;

	switch (mode)
	{
	case 1:
		map[begin.GetX()][begin.GetY()]->SetType(Empty);
		begin.SetPosition(player->Position);
		begin.x = begin.position.x / 50;
		begin.y = begin.position.y / 50;
		map[begin.x][begin.y]->SetType(Begin);

		map[destination.GetX()][destination.GetY()]->SetType(Empty);
		destination.SetPosition(D3DXVECTOR2(x, y));
		destination.x = int(x) / 50;
		destination.y = int(y) / 50;
		map[destination.x][destination.y]->SetType(Destination);
		break;

	case 2:		
		if (!_isPlayerMoving)
		{
			ResetScene();
			map[begin.GetX()][begin.GetY()]->SetType(Empty);
			begin.SetPosition(player->Position);
			begin.x = begin.position.x / 50;
			begin.y = begin.position.y / 50;
			map[begin.x][begin.y]->SetType(Begin);

			map[destination.GetX()][destination.GetY()]->SetType(Empty);
			destination.SetPosition(D3DXVECTOR2(x, y));
			destination.x = int(x) / 50;
			destination.y = int(y) / 50;
			map[destination.x][destination.y]->SetType(Destination);

			RunAStar();
			DrawPath();
			_isPlayerMoving = true;
		}
		else
		{
			if (currentNodeIndex > 0)
				player->Stop(path[currentNodeIndex-1]->position, path[currentNodeIndex]->position);
			else
				player->Stop(path[currentNodeIndex]->position, path[currentNodeIndex]->position);
			_isPlayerMoving = false;

			ResetScene();
			map[begin.GetX()][begin.GetY()]->SetType(Empty);
			begin.SetPosition(player->Position);
			begin.x = begin.position.x / 50;
			begin.y = begin.position.y / 50;
			map[begin.x][begin.y]->SetType(Begin);

			map[destination.GetX()][destination.GetY()]->SetType(Empty);
			destination.SetPosition(D3DXVECTOR2(x, y));
			destination.x = int(x) / 50;
			destination.y = int(y) / 50;
			map[destination.x][destination.y]->SetType(Destination);

			RunAStar();
			DrawPath();
			_isPlayerMoving = true;
		}
		break;

	case 3:
		int _x = int(x) / 50, _y = int(y) / 50;
		int id = _x + _y * (WIDTH / X_STEP);
		obstaclesNode.erase(id);
		map[_x][_y]->SetType(Empty);
		break;
	}
}

void GridScene::ResetScene()
{
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			if (map[x][y]->type != Obstacle)
				map[x][y]->SetType(Empty);
			else
				map[x][y]->SetType(Obstacle);
		}
	}
	path.clear();
}

void GridScene::RunAStar()
{
	path.clear();
	Node beginNode;
	//Setting position for begin node.
	beginNode.SetX(begin.GetX());
	beginNode.SetY(begin.GetY());

	Node destNode;
	destNode.SetX(destination.GetX());
	destNode.SetY(destination.GetY());
	std::vector<Node> result = AStar::aStar(beginNode, destNode, map);
	for (std::vector<Node>::iterator it = result.begin(); it != result.end(); it = std::next(it))
	{
		path.push_back(map[it->GetX()][it->GetY()]);
	}
}

void GridScene::DrawPath()
{
	if (!path.empty())
		for (int index = 0; index < path.size(); index++)
		{
			path[index]->SetType(Path);
		}
	else
		GAMELOG("Path is empty!");
}

void GridScene::GivePlayerOrder()
{
	if (_isPlayerMoving)
	{
		if (currentNodeIndex < path.size())
		{
			player->Move(path[currentNodeIndex]->position);
			if (!player->IsMoving()) currentNodeIndex++;
		}
		else
		{
			currentNodeIndex = 0;
			_isPlayerMoving = false;
		}
	}
}

void GridScene::Draw()
{
	for (int x = 0; x < (WIDTH / X_STEP); x++)
	{
		for (int y = 0; y < (HEIGHT / Y_STEP); y++)
		{
			map[x][y]->Draw();
		}
	}
	
	player->Draw();
}