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
#include "FibonacciHeap.h"

enum Part_Driection
{
	Part_Dir_Left_Top,
	Part_Dir_Right_Top,
	Part_Dir_Left_Bottom,
	Part_Dir_Right_Bottom
};

class PartResuit
{
public:
	Vec2 vec;
	Part_Driection partDir; //quét điểm từ 4 gốc
};


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
	
	std::array < std::array<int, COUNT_Y>, COUNT_X> map;
	std::array<std::array<GridTile*, COUNT_Y>, COUNT_X> mapGrid;

	std::vector<Vec2> listPosValids;

public:
	static Astar* getInstance();
	
	void Init();
	void SetValue(int x, int y, int value);
	AstarResuit findPath(Node player, Node dest);
	

	bool RandomPosValid(Vec2 *pos);  //random lấy tọa độ rỗng
	bool RandomoPosValidAround(Vec2 pos0, Vec2 *pos, int range); //random xung quanh 1 điểm với bán kính
	//get list node valid 
	std::vector<Vec2*> GetListVecInAxisValid(int x, int y, int xRound, int yRound);
	Vec2* RandomVecInAxisValid(Vec2 vecMine, Vec2 vecOther, int range);
public:
	bool isValid(int x, int y);
	bool isObstacle(int x, int y, std::array < std::array<int, COUNT_Y>, COUNT_X> map);
	bool isDestination(int x, int y, Node dest);
	double calculateH(int x, int y, Node dest);

	bool isValidAndNotObs(int x, int y);

	std::vector<Node> makePath(std::array<std::array<Node, COUNT_Y>, COUNT_X> map, Node dest);	

public:
	//Part
	std::array < std::array<int, COUNT_Y / SIZE_PART>, COUNT_X/ SIZE_PART> mapPart;
	AstarResuit findPathV2(Node player, Node dest);
	AstarResuit findPathV2WithPart(Node begin, Node dest, Vec2 partPlayer, Vec2 partDest, int size_part);
	std::vector<Node> findPathNodeV2(Node player, Node dest, int size_part);
	PartResuit getPartNext(Vec2 partPlayer, Vec2 partDest);
	//sử dụng với map to, chia ra từng ô
	//di chuyển từng ô to
	//path = đường nối giữa các ô to (ô to cuối cùng đi thẳng đến điểm cần đến)

	vector<Node> insertPath(vector<Node> path1, vector<Node> path2);
};


