#include "GraphNode.h"

bool Node::operator < (const Node& n) const
{
	return fCost < n.fCost;
}

bool Node::operator > (const Node& n) const
{
	return fCost > n.fCost;
}

std::ostream& operator << (std::ostream& os, const Node& n)
{
	os << "(" << n.x << "," << n.y << ")";
	return os;
}