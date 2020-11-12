#pragma once

#include <iostream>
#include "GridTile.h"

class Node
{
	int x, y;
	int parentX, parentY;
	float gCost;
	float hCost;
	float fCost;
	//GridTile value;
	friend class AStar;
public:
	int GetX() { return x; }
	int GetY() { return y; }
	void SetX(int value) { x = value; }
	void SetY(int value) { y = value; }
	bool operator < (const Node& n) const;
	bool operator > (const Node& n) const;
	friend std::ostream& operator << (std::ostream& os, const Node& n);
};