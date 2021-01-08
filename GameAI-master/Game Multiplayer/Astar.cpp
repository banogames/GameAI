#pragma once
#include"Astar.h";      /* time */
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */

// Define the static Singleton pointer
Astar* Astar::inst_ = NULL;

Astar* Astar::getInstance() {
	if (inst_ == NULL) {
		inst_ = new Astar();
		inst_->Init();
	}
	return(inst_);
}



bool Astar::isValid(int x, int y)
{ //If is not valid
	if (x < 0 || y < 0 || x >= COUNT_X || y >= COUNT_Y) {
		return false;
	}
	return true;
}


bool Astar::isObstacle(int x, int y, std::array < std::array<int, COUNT_Y>, COUNT_X> map)
{	
	if (isValid(x, y)) {
		return map[x][y] != 0;
	}
	return false;
}

bool Astar::isValidAndNotObs(int x, int y)
{
	return isValid(x, y) && !isObstacle(x, y, map);
}

bool Astar::isDestination(int x, int y, Node dest) {
		if (x == dest.x && y == dest.y) {
			return true;
		}
		return false;
	}

double Astar::calculateH(int x, int y, Node dest) {
		double H = (sqrt((x - dest.x)*(x - dest.x)
			+ (y - dest.y))*(y - dest.y));
		return H;
}

void Astar::Init() 
{
	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
		{
			mapGrid[x][y] = new GridTile();
			mapGrid[x][y]->SetVec(x, y);
			mapGrid[x][y]->SetPosition(D3DXVECTOR2((x + 0.5f) * X_STEP, (y + 0.5f) * Y_STEP));
			map[x][y] = 0;
		}
	}
}

void Astar::SetValue(int x, int y, int value)
{
	if (isValid(x, y)) 
	{
		map[x][y] = value;
	}
}

std::vector<Node> Astar::makePath(std::array<std::array<Node, COUNT_Y>, COUNT_X> map, Node dest)
{
	try
	{
		std::cout << "Found a path!" << std::endl;
		int x = dest.x;
		int y = dest.y;
		std::stack<Node> path;
		std::vector<Node> validPath;
		while (!(map[x][y].parentX == x && map[x][y].parentY == y)
			&& map[x][y].x != -1 && map[x][y].y != -1)
		{
			path.push(map[x][y]);
			int tempX = map[x][y].parentX;
			int tempY = map[x][y].parentY;
			x = tempX;
			y = tempY;
		}
		path.push(map[x][y]);

		while (!path.empty())
		{
			Node top = path.top();
			path.pop();
			validPath.emplace_back(top);
		}

		return validPath;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

AstarResuit Astar::findPath(Node begin, Node dest)
{
	AstarResuit resuit;

	if (!isValid(dest.x, dest.y)) {
		std::cout << "Destination is not valid" << std::endl;
		return resuit;
	}

	if (isObstacle(dest.x, dest.y, map)) 
	{
		std::cout << "Destination is an obstacle" << std::endl;
		return resuit;
	}

	if (isDestination(begin.x, begin.y, dest)) {
		std::cout << "You are the destination" << std::endl;
		return resuit;
	}

	bool closedList[COUNT_X][COUNT_Y];

	//Initialize whole map
	std::array<std::array < Node, COUNT_Y>, COUNT_X> _mapNode;
	for (int x = 0; x < COUNT_X; x++) {
		for (int y = 0; y < COUNT_Y; y++) {
			_mapNode[x][y].fCost = FLT_MAX;
			_mapNode[x][y].gCost = FLT_MAX;
			_mapNode[x][y].hCost = FLT_MAX;
			_mapNode[x][y].parentX = -1;
			_mapNode[x][y].parentY = -1;
			_mapNode[x][y].x = x;
			_mapNode[x][y].y = y;

			closedList[x][y] = false;
		}
	}

	//Initialize our starting list
	int x = begin.x;
	int y = begin.y;
	_mapNode[x][y].fCost = 0.0;
	_mapNode[x][y].gCost = 0.0;
	_mapNode[x][y].hCost = 0.0;
	_mapNode[x][y].parentX = x;
	_mapNode[x][y].parentY = y;

	//mở note đầu tiên
	
	resuit.openList.emplace_back(_mapNode[x][y]);
	bool destinationFound = false;

#pragma region  Open fibonacciheap
	FibonacciHeap<Node> openList;

	while (!openList.isEmpty() && openList.size() < COUNT_X * COUNT_Y)
	{
		Node node;
		do
		{
			node = openList.removeMinimum();
		} while (!openList.isEmpty() && !isValidAndNotObs(node.x, node.y));


		x = node.x;
		y = node.y;
		closedList[x][y] = true;
		//mapGrid[x][y]->SetType(Closed);

		for (int newX = -1; newX <= 1; newX++)
		{
			for (int newY = -1; newY <= 1; newY++)
			{
				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY))
				{
					if (isDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0))
					{
						//Destination found - make path.
						_mapNode[x + newX][y + newY].parentX = x;
						_mapNode[x + newX][y + newY].parentY = y;
						destinationFound = true;
						resuit.path = makePath(_mapNode, dest);
						return resuit;
					}
					else if (!closedList[x + newX][y + newY])
					{
						if (newX == 0 || newY == 0)
							gNew = node.gCost + 1.5;
						else
							continue;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;

						//Check if this path is better than the one already present.
						if (_mapNode[x + newX][y + newY].fCost == FLT_MAX || _mapNode[x + newX][y + newY].fCost > fNew)
						{
							_mapNode[x + newX][y + newY].fCost = fNew;
							_mapNode[x + newX][y + newY].gCost = gNew;
							_mapNode[x + newX][y + newY].hCost = hNew;
							_mapNode[x + newX][y + newY].parentX = x;
							_mapNode[x + newX][y + newY].parentY = y;
							openList.insert(_mapNode[x + newX][y + newY]);
							//mapGrid[x + newX][y + newY]->SetType(Opened);
						}
					}
				}
			}
		}
		//PrintMatrix(_mapNode);
	}

#pragma endregion

	if (destinationFound == false) {
		std::cout << "Destination not found" << std::endl;
		return resuit;
	}
	return resuit;
}


bool Astar::RandomPosValid(Vec2 *pos)
{
	//random ô (x,y)
	int partX = rand() % (NUM_PART_X);
	int partY = rand() % (NUM_PART_Y);
	//random vị trí trong ô

	Vec2 posPart;
	posPart.x = (partX + 0.5f)*SIZE_PART;
	posPart.y = (partY + 0.5f)*SIZE_PART;

	return RandomoPosValidAround(posPart, pos, SIZE_PART);
}

bool Astar::RandomoPosValidAround(Vec2 pos0, Vec2 *pos, int range)
{
	std::vector<Vec2> listPosValids;  //list tọa độ rỗng

	int xMin, xMax, yMin, yMax;
	xMin = pos0.x - range;
	xMax = pos0.x + range;
	yMin = pos0.y - range;
	yMax = pos0.y + range;
	xMin = xMin < 0 ? 0 : xMin;
	xMax = xMax > COUNT_X ? COUNT_X : xMax;
	yMin = yMin < 0 ? 0 : yMin;
	yMax = yMax > COUNT_Y ? COUNT_Y : yMax;

	for (int x = xMin; x < xMax; x++)
	{
		for (int y = yMin; y < yMax; y++)
		{
			if (map[x][y] == 0)
				listPosValids.push_back(*new Vec2(x, y));
		}
	}
	if (!listPosValids.empty())
	{
		printLog("cout valids " + listPosValids.size());
		int index = rand() % listPosValids.size(); // pick a random index
		*pos = listPosValids[index]; // a random value taken from that list
		return true;
	}
	else {
		printLog("cout valids empty");
	}
	return false;
}

std::vector<Vec2*> Astar::GetListVecInAxisValid(int x, int y, int xRound, int yRound, int counBrick)
{
	std::vector<Vec2*> validList;

	//xét bề ngang
	//quét từ player ra 2 bên, cho phép bắn qua 1 viên gạch
	int _countBrick = 0;

	int _dtX = xRound > x ? 1 : -1;
	while (xRound!=x && _countBrick <=counBrick)
	{
		if (isValid(x, y))
		{
			if (!isObstacle(x, y, map)) {
				Vec2* vec = new Vec2(x, y);
				validList.push_back(vec);
			}
			else
			{
				_countBrick++;
			}
		}
		
		x += _dtX;
	}

	_countBrick = 0;
	//xét bề dọc
	int _dtY = yRound > y ? 1 : -1;
	while (yRound!=y && _countBrick <= counBrick)
	{
		if (isValid(x, y))
		{
			if (!isObstacle(x, y, map)) {
				Vec2* vec = new Vec2(x, y);
				validList.push_back(vec);
			}
			else
			{
				_countBrick++;
			}
		}
		y += _dtY;
	}

	if (validList.empty()) return GetListVecInAxisValid(x, y, xRound, yRound, counBrick + 1);
	return validList;
}

double calculateVecH(Vec2 vec0, Vec2 vec1) {
	double H = (sqrt((vec0.x - vec1.x)*(vec0.x - vec1.x)
		+ (vec0.y - vec1.y))*(vec0.y - vec1.y));
	return H;
}

Vec2* Astar::RandomVecInAxisValid(Vec2 vecMine, Vec2 vecOther, int range)
{
	int xRound = vecMine.x < vecOther.x ? vecMine.x - range : vecOther.x + range;
	int yRound = vecMine.y < vecOther.y ? vecMine.y - range : vecMine.y + range;
	std::vector<Vec2*> validList = GetListVecInAxisValid(vecOther.x, vecOther.y, xRound, yRound, 0);
	Vec2* vecResult = new Vec2(vecOther.x, vecOther.y);
	double dirResult;
	if (!validList.empty())
	{
		vecResult = validList.at(0);
		dirResult = calculateVecH(vecMine, *vecResult);
		for (auto vec : validList)
		{
			if (calculateVecH(vecMine, *vec) < dirResult)
			{
				vecResult = vec;
				dirResult = calculateVecH(vecMine, *vec);
			}
		}
	}
	return vecResult;
}

PartResuit Astar::getPartNext(Vec2 partPlayer, Vec2 partDest)
{
	PartResuit resuit;

	//lấy part player làm gốc tọa độ
	int deltaX = partDest.x - partPlayer.x;
	int deltaY = partDest.y - partPlayer.y;

	//chọn trong những ô có thể đi ra ô kế tiếp
	//(dựa theo gốc chọn ô kế tiếp)
	//lấy node kế tiếp
	//đi tới node kế tiếp

	float absTan = deltaX==0? deltaY : abs(deltaY / deltaX);

	if (deltaX > 0)
	{
		if (deltaY > 0)
		{
			resuit.partDir = Part_Dir_Left_Top;
			//3 ô 
			//[x+1, y] [x+1,y+1] [x, y+1]
			if (absTan <= 0.33f)
			{
				//[x+1, y]
				resuit.vec.x = partPlayer.x + 1;
				resuit.vec.y = partPlayer.y;
			}
			else if (absTan <= 3)
			{
				//[x+1,y+1]
				resuit.vec.x = partPlayer.x + 1;
				resuit.vec.y = partPlayer.y + 1;
			}
			else
			{
				//[x, y+1]
				resuit.vec.x = partPlayer.x;
				resuit.vec.y = partPlayer.y + 1;
			}
		}
		else
		{
			resuit.partDir = Part_Dir_Left_Bottom;
			//3 ô 
			//[x+1, y] [x+1,y-1] [x, y-1]
			if (absTan <= 0.33f)
			{
				//[x+1, y]
				resuit.vec.x = partPlayer.x + 1;
				resuit.vec.y = partPlayer.y;
			}
			else if (absTan <= 3)
			{
				//[x+1,y-1]
				resuit.vec.x = partPlayer.x + 1;
				resuit.vec.y = partPlayer.y - 1;
			}
			else
			{
				//[x, y-1]
				resuit.vec.x = partPlayer.x;
				resuit.vec.y = partPlayer.y - 1;
			}
		}
	}
	else
	{
		if (deltaY > 0)
		{
			resuit.partDir = Part_Dir_Right_Top;
			//3 ô
			//[x-1, y] [x-1,y+1] [x, y+1]
			if (absTan <= 0.33f)
			{
				//[x-1, y]
				resuit.vec.x = partPlayer.x - 1;
				resuit.vec.y = partPlayer.y;
			}
			else if (absTan <= 3)
			{
				//[x-1,y+1]
				resuit.vec.x = partPlayer.x - 1;
				resuit.vec.y = partPlayer.y + 1;
			}
			else
			{
				//[x, y+1]
				resuit.vec.x = partPlayer.x;
				resuit.vec.y = partPlayer.y + 1;
			}
		}
		else
		{
			resuit.partDir = Part_Dir_Right_Bottom;
			//3 ô
			//[x-1, y] [x-1,y-1] [x, y -1]
			if (absTan < 0.33f)
			{
				//[x-1, y]
				resuit.vec.x = partPlayer.x - 1;
				resuit.vec.y = partPlayer.y;
			}
			else if (absTan < 3)
			{
				//[x-1,y-1]
				resuit.vec.x = partPlayer.x - 1;
				resuit.vec.y = partPlayer.y - 1;
			}
			else
			{
				//[x, y-1]
				resuit.vec.x = partPlayer.x;
				resuit.vec.y = partPlayer.y -1;
			}
		}
	}

	return resuit;
}

std::vector<Node> Astar::insertPath(vector<Node> path1, vector<Node> path2) 
{
	path1.insert(path1.end(), path2.begin(), path2.end());
	return path1;
}

std::vector<Node> Astar::findPathNodeV2(Node player, Node dest, int size_part)
{	
	//nếu khoảng cách bé hơn 1 ô
	if (abs(player.x - dest.x) <= size_part && abs(player.y - dest.y) <= size_part) 
	{
		Vec2 partPlayer, partDest;
		partPlayer.x = player.x / size_part;
		partPlayer.y = player.y / size_part;

		partDest.x = dest.x / size_part;
		partDest.y = dest.y / size_part;

		return findPathV2WithPart(player, dest, partPlayer, partDest, size_part).path;
	}

	Vec2 partPlayer, partDest;
	partPlayer.x = player.x / size_part;
	partPlayer.y = player.y / size_part;

	partDest.x = dest.x / size_part;
	partDest.y = dest.y / size_part;

	if (partPlayer.x == partDest.x && partPlayer.y == partDest.y) 
	{
		//cùng 1 ô
		return findPathV2WithPart(player, dest, partPlayer, partDest, size_part).path;
	}
	else //nếu ở khác ô => kiếm ô cần đến
	{
		Node next;
		PartResuit partNext = getPartNext(partPlayer, partDest);

		//lấy ô valid trong part
		//quét theo hướng

		switch (partNext.partDir)
		{
		case Part_Dir_Left_Top:
			printLog("Move left top");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part; x <= partNext.vec.x * size_part + i; x++)
				{
					for (int y = partNext.vec.y * size_part + i; y >= partNext.vec.y * size_part; y--)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}

		case Part_Dir_Left_Bottom:
			printLog("Move left bottom");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part + i; x >= partNext.vec.x * size_part; x--)
				{
					for (int y = partNext.vec.y * size_part; y <= partNext.vec.y * size_part + i; y++)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}

		case Part_Dir_Right_Top:
			printLog("Move right top");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part - i; x <= partNext.vec.x * size_part; x++)
				{
					for (int y = partNext.vec.y * size_part; y <= partNext.vec.y * size_part + i; y++)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}

		case Part_Dir_Right_Bottom:
			printLog("Move right bottom");
			for (int i = 0; i <= size_part; i++)
			{
				for (int x = partNext.vec.x * size_part; x >= partNext.vec.x * size_part - i; x--)
				{
					for (int y = partNext.vec.y * size_part - i; y <= partNext.vec.y * size_part; y++)
					{
						if (isValidAndNotObs(x, y))
						{
							next.x = x;
							next.y = y;
							return findPathV2WithPart(player, next, partPlayer, partNext.vec, size_part).path;
						}
					}
				}
			}
		default:
			break;
		}

		return findPathNodeV2(player, dest, size_part + SIZE_PART);
	}

}

AstarResuit Astar::findPathV2(Node player, Node dest) 
{
	AstarResuit resuit;
	resuit.path = findPathNodeV2(player, dest, SIZE_PART);
	return resuit;
}



AstarResuit Astar::findPathV2WithPart(Node begin, Node dest, Vec2 partPlayer, Vec2 partDest, int size_part)
{
	AstarResuit resuit;

	if (!isValid(dest.x, dest.y)) {
		std::cout << "Destination is not valid" << std::endl;
		return resuit;
	}

	if (isObstacle(dest.x, dest.y, map))
	{
		std::cout << "Destination is an obstacle" << std::endl;
		return resuit;
	}

	if (isDestination(begin.x, begin.y, dest)) {
		std::cout << "You are the destination" << std::endl;
		return resuit;
	}

	int xMin, xMax, yMin, yMax;

	if (partPlayer.x < partDest.x) {
		xMin = partPlayer.x * size_part;
		xMax = (partDest.x + 1)*size_part;
	}
	else
	{
		xMin = partDest.x * size_part;
		xMax = (partPlayer.x + 1)*size_part;
	}

	if (partPlayer.y < partDest.y) {
		yMin = partPlayer.y * size_part;
		yMax = (partDest.y + 1)*size_part;
	}
	else
	{
		yMin = partDest.y * size_part;
		yMax = (partPlayer.y + 1)*size_part;
	}

	if (xMax > COUNT_X) xMax = COUNT_X;
	if (yMax > COUNT_Y) yMax = COUNT_Y;

	//Initialize whole map
	std::array<std::array < Node, COUNT_Y>, COUNT_X> _mapNode;
	bool closedList[COUNT_Y][COUNT_X];

	while (size_part < COUNT_Y/2)
	{
		for (int x = xMin; x < xMax; x++) {
			for (int y = yMin; y <  yMax; y++) {
				_mapNode[x][y].fCost = FLT_MAX;
				_mapNode[x][y].gCost = FLT_MAX;
				_mapNode[x][y].hCost = FLT_MAX;
				_mapNode[x][y].parentX = -1;
				_mapNode[x][y].parentY = -1;
				_mapNode[x][y].x = x;
				_mapNode[x][y].y = y;

				closedList[x][y] = false;
			}
		}

		//Initialize our starting list
		int x = begin.x;
		int y = begin.y;
		_mapNode[x][y].fCost = 0.0;
		_mapNode[x][y].gCost = 0.0;
		_mapNode[x][y].hCost = 0.0;
		_mapNode[x][y].parentX = x;
		_mapNode[x][y].parentY = y;

		//mở note đầu tiên

		resuit.openList.emplace_back(_mapNode[x][y]);

		bool destinationFound = false;


	#pragma region  Open fibonacciheap
	FibonacciHeap<Node> openList;

	openList.insert(_mapNode[x][y]);
		
	while (!openList.isEmpty() && openList.size() < COUNT_X * COUNT_Y)
	{
		Node node;
		do
		{
			node = openList.removeMinimum();

		} while (!openList.isEmpty() && !isValidAndNotObs(node.x, node.y));
		
		x = node.x;
		y = node.y;
		closedList[x][y] = true;
		//mapGrid[x][y]->SetType(Closed);
		
		for (int newX = -1; newX <= 1; newX++)
		{
			for (int newY = -1; newY <= 1; newY++)
			{
				if (x + newX < xMin || x + newX > xMax
					|| y + newY < yMin || y + newY > yMax) continue;			

				double gNew, hNew, fNew;
				if (isValid(x + newX, y + newY))
				{
					if (isDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0))
					{
						//Destination found - make path.
						_mapNode[x + newX][y + newY].parentX = x;
						_mapNode[x + newX][y + newY].parentY = y;
						destinationFound = true;
						resuit.path = makePath(_mapNode, dest);
						return resuit;
					}
					else if (!closedList[x + newX][y + newY])
					{
						if (newX == 0 || newY == 0)
							gNew = node.gCost + 4.5f;
						else
							continue;
						hNew = calculateH(x + newX, y + newY, dest);
						fNew = gNew + hNew;
		
						//Check if this path is better than the one already present.
						if (_mapNode[x + newX][y + newY].fCost == FLT_MAX || _mapNode[x + newX][y + newY].fCost > fNew)
						{
							_mapNode[x + newX][y + newY].fCost = fNew;
							_mapNode[x + newX][y + newY].gCost = gNew;
							_mapNode[x + newX][y + newY].hCost = hNew;
							_mapNode[x + newX][y + newY].parentX = x;
							_mapNode[x + newX][y + newY].parentY = y;
							openList.insert(_mapNode[x + newX][y + newY]);
							//mapGrid[x + newX][y + newY]->SetType(Opened);
						}
					}
				}
			}
		}
		//PrintMatrix(_mapNode);
	}
		
#pragma endregion

		if (destinationFound == false) {
			std::cout << "Destination not found" << std::endl;
			return resuit;
		}

		size_part +=SIZE_PART;
	}

	return resuit;
}

