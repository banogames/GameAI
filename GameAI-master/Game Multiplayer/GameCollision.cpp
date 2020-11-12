#include "GameCollision.h"

bool GameCollision::IsCollideInNextFrame(Entity *entity1, Entity *entity2, float dt) {

	D3DXVECTOR2 currentPosition1 = entity1->Position;
	D3DXVECTOR2 currentPosition2 = entity2->Position;

	// cho 2 đối tượng cập nhật vị trí 1 frame
	entity1->Position = entity1->Position + entity1->Velocity * dt;
	entity2->Position = entity2->Position + entity2->Velocity * dt;

	// lấy 2 hcn của frame đó
	RECT rect1 = entity1->getBound();
	RECT rect2 = entity2->getBound();

	// đưa 2 đối tượng về lại vị trí ban đầu
	entity1->Position = currentPosition1;
	entity2->Position = currentPosition2;

	// trả về - frame tiếp theo có va chạm hay không
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top) {

		return false;
	}
	else
	{
		return true;
	}
}

CollisionResult GameCollision::getCollisionResult(Entity * entity1, Entity * entity2)
{
	CollisionResult result;

	RECT rect1 = entity1->getBound();
	RECT rect2 = entity2->getBound();

	if (rect1.left <= rect2.right &&
		rect1.right >= rect2.left &&
		rect1.top <= rect2.bottom &&
		rect1.bottom >= rect2.top)
	{
		result.IsCollided = true;
	}
	else
	{
		result.IsCollided = false;
		// không va chạm thì không xét nữa
		return result;
	}

	// xác định RECT
	result.Rect.left = rect1.left > rect2.left ? rect1.left : rect2.left;
	result.Rect.right = rect1.right < rect2.right ? rect1.right : rect2.right;
	result.Rect.top = rect1.top > rect2.top ? rect1.top : rect2.top;
	result.Rect.bottom = rect1.bottom < rect2.bottom ? rect1.bottom : rect2.bottom;

	// xác định phía va chạm
	D3DXVECTOR2 collisionCenter = D3DXVECTOR2(
		(result.Rect.left + result.Rect.right) / 2.0f,
		(result.Rect.top + result.Rect.bottom) / 2.0f);
	// lấy vector từ e1 đến RECT va chạm
	D3DXVECTOR2 dir = collisionCenter - entity1->Position;
	D3DXVec2Normalize(&dir, &dir);

	float a1 = 0.6427876f; // cos(50 độ)
	float a2 = 0.7660444f; // cos(40 độ)

	if (dir.y < 0)
	{
		if (dir.x <= a1 && dir.x >= -a1)
		{
			result.Side = CS_Top;
		}
		else if (dir.x > a2)
		{
			result.Side = CS_Right;
		}
		else if (dir.x < -a2)
		{
			result.Side = CS_Left;
		}
	}
	else
	{
		if (dir.x <= a1 && dir.x >= -a1)
		{
			result.Side = CS_Bottom;
		}
		else if (dir.x > a2)
		{
			result.Side = CS_Right;
		}
		else if (dir.x < -a2)
		{
			result.Side = CS_Left;
		}
	}

	return result;
}
