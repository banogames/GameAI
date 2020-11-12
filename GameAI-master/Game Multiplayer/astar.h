#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <map> //dictionay
#include "GameDefine.h"
#include "Node.h"
#include "Vec2.h"

using namespace std;


class Astar
{
	public:
		static vector<Node*> maps;
		static map<int, Node*>  dicObstacles;  //id + node
		static Node allMap[COUNT_X][COUNT_Y];

	public:
		static bool isValid(int x, int y);
		static bool isObstacle(int x, int y);
		static bool isDestination(int x, int y, Node dest);
		static double calculateH(int x, int y, Node dest);
		static vector<Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest);
		static vector<Node> aStar(Node player, Node dest);

};

