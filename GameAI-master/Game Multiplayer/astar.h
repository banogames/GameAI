#pragma once
#include <iostream>
#include <stack>
#include <vector>
#include <list>
#include <array>
#include <set>
#include "GameDefine.h"
#include "GameLog.h"
#include "Node.h"
#include "GridTile.h";


class AstarResuit
{
public:
	std::vector<Node> path;
	std::vector<Node> closeList;
	std::vector<Node> openList;
};

class Astar
{
private:
	static Astar* inst_;   // The one, single instance
	Astar() 
	{} 
	Astar(const Astar&);
	Astar& operator=(const Astar&);

public:
	std::array < std::array<int, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map;
	std::array<std::array<GridTile*, COUNT_Y>, COUNT_X> mapGrid;

public:
	static Astar* getInstance();
	
	void Init();
	void SetValue(int x, int y, int value);
	AstarResuit findPath(Node player, Node dest);
	bool RandomPosValid(Vec2 *pos);  //random lấy tọa độ rỗng
	bool RandomoPosValidAround(Vec2 *pos0, Vec2 *pos, int radius); //random xung quanh 1 điểm với bán kính

public:
	bool isValid(int x, int y);
	bool isObstacle(int x, int y, std::array < std::array<int, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map);
	bool isDestination(int x, int y, Node dest);
	double calculateH(int x, int y, Node dest);

	bool isValidAndNotObs(int x, int y);

	std::vector<Node> makePath(std::array<std::array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest);

};


