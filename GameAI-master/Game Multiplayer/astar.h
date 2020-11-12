#include <iostream>
#include <stack>
#include <vector>
#include <array>
#include <map> //dictionay
#include "GameDefine.h"

using namespace std;




class Astar
{
public:


	struct Vec2
	{
		uint16_t x;
		uint16_t y;

		Vec2() : x(0), y(0)
		{
		}

		Vec2(uint16_t x1, uint16_t y1) : x(x1), y(y1)
		{
		}

		void reset(uint16_t x1, uint16_t y1)
		{
			x = x1;
			y = y1;
		}

		int distance(const Vec2 &other) const
		{
			return abs(other.x - x) + abs(other.y - y);
		}

		bool operator== (const Vec2 &other) const
		{
			return x == other.x && y == other.y;
		}
	};


	struct Node
	{
		Astar::Vec2 vec;
		Astar::Vec2 vecParent;
		float gCost;
		float hCost;
		float fCost;

		Node() : vec(0,0), vecParent(0,0), gCost(0), hCost(0), fCost(0)
		{}

		Node(){}

		//bool operator< (const Node& lhs, const Node& rhs) const
		//{//We need to overload "<" to put our struct into a set
		//	return lhs.fCost < rhs.fCost;
		//};
	};

	


	public:
		static vector<Astar::Node*> maps;
		static map<int, Astar::Node*>  dicObstacles;  //id + node
		static Astar::Node allMap[COUNT_X][COUNT_Y];

	public:
		static bool isValid(int x, int y);
		static bool isObstacle(int x, int y);
		static bool isDestination(int x, int y, Node dest);
		static double calculateH(int x, int y, Node dest);
		static vector<Astar::Node> makePath(array<array<Node, (Y_MAX / Y_STEP)>, (X_MAX / X_STEP)> map, Node dest);
		static vector<Astar::Node> aStar(Node player, Node dest);

};

