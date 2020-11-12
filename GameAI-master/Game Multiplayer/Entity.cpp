#include "Entity.h"

RECT Entity::getBound()
{
	RECT bound;

	bound.left = (long)(Position.x - _width / 2.f);
	bound.right = (long)(Position.x + _width / 2.f);
	bound.top = (long)(Position.y - _height / 2.f);
	bound.bottom = (long)(Position.y + _height / 2.f);

	return bound;
}