#pragma once
#include"Astar.h";

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
{ //If our Node is an obstacle it is not valid
	if (x < 0 || y < 0 || x >= (X_MAX / X_STEP) || y >= (Y_MAX / Y_STEP)) {
		return false;
	}
	return true;
}


bool Astar::isObstacle(int x, int y, std::array < std::array<int, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map)
{	
	if (isValid(x, y)) {
		return map[x][y] != 0;
	}
	return false;
}

bool Astar::isValidAndNotObs(int x, int y)
{
	return isValid(x, y) && isObstacle(x, y, map);
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

std::vector<Node> Astar::makePath(std::array<std::array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest)
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

	bool closedList[(X_MAX / X_STEP)][(Y_MAX / Y_STEP)];

	//Initialize whole map
	std::array<std::array < Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> _mapNode;
	for (int x = 0; x < (X_MAX / X_STEP); x++) {
		for (int y = 0; y < (Y_MAX / Y_STEP); y++) {
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

	while (!resuit.openList.empty() && resuit.openList.size() < (X_MAX / X_STEP)*(Y_MAX / Y_STEP)) {
		Node node;
		do {
			//This do-while loop could be replaced with extracting the first
			//element from a set, but you'd have to make the resuit.openList a set.
			//To be completely honest, I don't remember the reason why I do
			//it with a vector, but for now it's still an option, although
			//not as good as a set performance wise.
			float temp = FLT_MAX;
			std::vector<Node>::iterator itNode = resuit.openList.begin();
			for (std::vector<Node>::iterator it = resuit.openList.begin();
				it != resuit.openList.end(); it = next(it)) {
				Node n = *it;
				//lấy node với fCost bé hơn
				if (n.fCost < temp) {
					temp = n.fCost;
					itNode = it;
				}
			}
			node = *itNode;
			resuit.openList.erase(itNode);
		} while (isValid(node.x, node.y) == false);

		x = node.x;
		y = node.y;
		closedList[x][y] = true;
		resuit.closeList.push_back(node);

#pragma region  for xung quanh
for (int newX = -1; newX <= 1; newX++)
{
	for (int newY = -1; newY <= 1; newY++)
	{
		double gNew, hNew, fNew;
		if (isValid(x + newX, y + newY)) //kiểm tra hợp lệ
		{
			if (!isObstacle(x + newX, y + newY, map)) //kiểm tra tồn tại
			{
				if (isDestination(x + newX, y + newY, dest) && (newX == 0 || newY == 0)) //tới điểm cuối
				{
					//Destination found - make path
					_mapNode[x + newX][y + newY].parentX = x;
					_mapNode[x + newX][y + newY].parentY = y;
					destinationFound = true;
					resuit.path = makePath(_mapNode, dest);
					return resuit;
				}
				else if (closedList[x + newX][y + newY] == false)
				{
					if (newX == 0 || newY == 0)
						gNew = node.gCost + 1;
					else continue;
					hNew = calculateH(x + newX, y + newY, dest);
					fNew = gNew + hNew;
					 //Check if this path is better than the one already present
					if (_mapNode[x + newX][y + newY].fCost == FLT_MAX ||
						_mapNode[x + newX][y + newY].fCost > fNew)
					{
						//Update the details of this neighbour node
						_mapNode[x + newX][y + newY].fCost = fNew;
						_mapNode[x + newX][y + newY].gCost = gNew;
						_mapNode[x + newX][y + newY].hCost = hNew;
						_mapNode[x + newX][y + newY].parentX = x;
						_mapNode[x + newX][y + newY].parentY = y;
						resuit.openList.emplace_back(_mapNode[x + newX][y + newY]);
					}
				}
			}
		}
	}
}
#pragma endregion
		
	}
	if (destinationFound == false) {
		std::cout << "Destination not found" << std::endl;
		return resuit;
	}
	return resuit;
}


bool Astar::RandomPosValid(Vec2 *pos)
{
	std::vector<Vec2> listPosValids;  //list tọa độ rỗng
	for (int x = 0; x < COUNT_X; x++)
	{
		for (int y = 0; y < COUNT_Y; y++)
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

bool Astar::RandomoPosValidAround(Vec2 *pos0, Vec2 *pos, int radius)
{
	std::vector<Vec2> listPosValids;  //list tọa độ rỗng

	int xMin, xMax, yMin, yMax;
	xMin = pos0->x - radius;
	xMax = pos0->x + radius;
	yMin = pos0->y - radius;
	yMax = pos0->y + radius;
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

