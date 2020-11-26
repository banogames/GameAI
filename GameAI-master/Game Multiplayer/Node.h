#pragma once

#include <iostream>

class Node
{
public:
	int x, y;
	int parentX, parentY;
	float gCost;
	float hCost;
	float fCost;
	friend class Astar;
public:
	void SetX(int value) { x = value; }
	void SetY(int value) { y = value; }

	bool Node::operator < (const Node& n) const
	{
		return fCost < n.fCost;
	}

	bool Node::operator > (const Node& n) const
	{
		return fCost > n.fCost;
	}
};