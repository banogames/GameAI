#include "NPC.h"

#include "SpriteList.h"
#include "GameCollision.h"
#include "GameDefine.h"
#include "Astar.h"

NPC::NPC()
{
	_type = ET_NPC;
	IsDeleted = false;
	_width = 25;
	_height = 25;

	// khởi tạo các animation
	_leftAnimation = new Animation();
	_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 82, 82 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	_rightAnimation = new Animation();
	_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 233, 233 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	_upAnimation = new Animation();
	_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 2, 2 + 32, 4, 4 + 32,
		D3DXVECTOR2(16.f, 16.f)));
	_downAnimation = new Animation();
	_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->TankAI, 156, 156 + 32, 2, 2 + 32,
		D3DXVECTOR2(16.f, 16.f)));

	// mặc định animation ban đầu
	_currentAnimation = _leftAnimation;

	Position = D3DXVECTOR2(300.f, 300.f);

	_isMoving = true;
}

void NPC::SetPosition(float x, float y) 
{
	Position = D3DXVECTOR2(x, y);
}

void NPC::Update(float _dt)
{
	if (IsDeleted)
		return;

	Position += Velocity * _dt;

	AutoMove(_dt);
}

void NPC::Draw()
{
	if (IsDeleted)
		return;

	_currentAnimation->Draw(Position);
}

void NPC::DrawPath() 
{
	//draw path
	if (!path.empty())
	{
		for (std::vector<GridTile*> ::iterator it = path.begin(); it != path.end(); it = next(it))
		{
			(*it)->Draw();
		}
	}
}

bool NPC::CheckCollision(Entity * e)
{
	if (IsDeleted)
		return false;

	CollisionResult cR = GameCollision::getCollisionResult(this, e);
	if (cR.IsCollided)
	{
		//dừng lại
		Stop();

		//tách ra khỏi va chạm
		if (cR.Side == CS_Left)
		{
			Position.x += (float)(cR.Rect.right - cR.Rect.left) + 10;
		}
		else if (cR.Side == CS_Right)
		{
			Position.x -= (float)(cR.Rect.right - cR.Rect.left) - 10;
		}
		else if (cR.Side == CS_Top)
		{
			Position.y += (float)(cR.Rect.bottom - cR.Rect.top) + 10;
		}
		else if (cR.Side == CS_Bottom)
		{
			Position.y -= (float)(cR.Rect.bottom - cR.Rect.top) - 10;
		}

		Astar::getInstance()->SetValue(Position.x / X_STEP, Position.y / Y_STEP, 2);

		D3DXVECTOR2 posOther = D3DXVECTOR2();
		posOther.x = cR.Rect.right - cR.Rect.left;
		posOther.y = cR.Rect.bottom - cR.Rect.top;

		/*Vec2 vecOther = Vec2();
		vecOther.x = (int)(posOther.x / X_STEP));
		vecOther.y = (int)(posOther.y / Y_STEP));

		posOther.x = vecOther.x * X_STEP;
		posOther.y = vecOther.y * Y_STEP;*/

		Astar::getInstance()->SetValue(posOther.x / X_STEP, posOther.y / Y_STEP, 2);
	
		//kiểm tra node hiện tại, node trước => valid sẽ tốc biến về đó
		if (!path.empty())
		{
			for (int i = -1; i <= 1; i++)
			{
				if (currentNodeIndex + i < path.size())
				{
					if (Astar::getInstance()->isValidAndNotObs(path.at(currentNodeIndex + i)->GetVec().x, path.at(currentNodeIndex + i)->GetVec().y))
					{
						Position = path.at(currentNodeIndex + i)->GetPosition();
						break;
					}
				}
			}
		}

		printLog("collision player");

		if (!_isCollision) 
		{
			RunDodging(); //chạy né va chạm		
		}
	}
	return cR.IsCollided;
}


void NPC::ApplyAnimation()
{
	switch (_direction)
	{
	case D_Left:
		_currentAnimation = _leftAnimation;
		break;
	case D_Right:
		_currentAnimation = _rightAnimation;
		break;
	case D_Up:
		_currentAnimation = _upAnimation;
		break;
	case D_Down:
		_currentAnimation = _downAnimation;
		break;
	default:
		break;
	}
}

D3DXVECTOR2 NPC::getVelocityByDirection(Direction direction)
{
	if (direction == D_Stand)
	{
		return D3DXVECTOR2(0, 0);
	}
	else if (direction == D_Left)
	{
		return D3DXVECTOR2(-_speed, 0);
	}
	else if (direction == D_Right)
	{
		return D3DXVECTOR2(_speed, 0);
	}
	else if (direction == D_Up)
	{
		return D3DXVECTOR2(0, -_speed);
	}
	else if (direction == D_Down)
	{
		return D3DXVECTOR2(0, _speed);
	}

	return D3DXVECTOR2(0, 0);
}

void NPC::ApplyVelocity()
{
	switch (_direction)
	{
	case D_Stand:
		Velocity = D3DXVECTOR2(0.f, 0.f);
		break;
	case D_Left:
		Velocity = D3DXVECTOR2(-_speed, 0.f);
		break;
	case D_Right:
		Velocity = D3DXVECTOR2(_speed, 0.f);
		break;
	case D_Up:
		Velocity = D3DXVECTOR2(0.f, -_speed);
		break;
	case D_Down:
		Velocity = D3DXVECTOR2(0.f, _speed);
		break;
	}
}

void NPC::AutoMove(float dt)
{
	if (!_isMoving && !_isCollision)
	{
		_timeWaittingMove += dt;
		if (_timeWaittingMove > 2)
		{
			RunAstar();
			return;
		}
	}

	if (_isCollision) 
	{
		Stop();
		return;
	}
	
	if (!path.empty())
	{
		if (currentNodeIndex >= path.size())
		{
			RunAstar();
		}
		else
		{			
			Move(path[currentNodeIndex]->GetPosition());
			if (!_isMoving) 
			{
				Astar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x, path[currentNodeIndex]->GetVec().y, 0);
				//bỏ old pos, add new pos
				currentNodeIndex++;
				if (currentNodeIndex < path.size())
					Astar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x, path[currentNodeIndex]->GetVec().y, 2);
			}
		}
	}
	else
	{
		RunAstar();
	}	
}

void NPC::PredictCollision() 
{
	if (_isMoving && !_isCollision) 
	{
		if (!path.empty())
		{
			//dự đoán những nốt tiếp theo có vật cản không, nếu có random lại vị trí di chuyển 1 path nhỏ
			//giữ des cũ, khi chạy xong des mới => chạy lại des cũ
			if (currentNodeIndex < path.size())
			{
				for (unsigned i = currentNodeIndex + 1; i < currentNodeIndex + 4; i++)
				{
					if (i >= path.size()) break;
					if (Astar::getInstance()->isValidAndNotObs(path.at(i)->GetVec().x, path.at(i)->GetVec().y))
					{
						RunDodging();
						break;
					}
				}
			}
		}
	}
}

void NPC::RunDodging() 
{
	Vec2 *pos = new Vec2();
	Vec2 *pos0 = new Vec2();
	pos0->x = Position.x / X_STEP;
	pos0->y = Position.y / Y_STEP;

	//random 1 node xung quanh, bán kính 5 ô
	if (Astar::getInstance()->RandomoPosValidAround(pos0, pos, 5)) {
		//random path xung quanh

		//lưu node cuối lại, sau khi end path né => astar tới node cuối
		if (!_isDodging) 
		{
			if (!path.empty()) {
				_desSave->x = (path.back())->GetVec().x;
				_desSave->y = (path.back())->GetVec().y;
			}
		}

		_isDodging = true;
		PathAstar(pos->x, pos->y);
	}
}


void NPC::RunAstar() 
{
	if (!_isDodging) //đang không phải né
	{
		Vec2 *pos = new Vec2();
		if (Astar::getInstance()->RandomPosValid(pos)) 
		{
			PathAstar(pos->x, pos->y);
		}
	}
	else //đã né xong, đi về chỗ đã lưu
	{
		_isDodging = false;
		//hết né, tính astar về chỗ ban đầu
		PathAstar(_desSave->x, _desSave->y);
	}
}

void NPC::PathAstar(int posX, int posY)
{
	currentNodeIndex = 0;

	Node beginNode;
	beginNode.SetX(Position.x / X_STEP);
	beginNode.SetY(Position.y / Y_STEP);

	Node destNode;
	destNode.SetX(posX);
	destNode.SetY(posY);

	AstarResuit resuit = Astar::getInstance()->findPath(beginNode, destNode);

	//draw closed, opened list
	/*for (std::vector<Node> ::iterator it = resuit.closeList.begin(); it != resuit.closeList.end(); it = next(it))
	{
		Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Closed);
	}

	for (std::vector<Node> ::iterator it = resuit.openList.begin(); it != resuit.openList.end(); it = next(it))
	{
		Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Opened);
	}
*/
	//set path
	path.clear();

	for (std::vector<Node> ::iterator it = resuit.path.begin(); it != resuit.path.end(); it = next(it))
	{
		if (!_isDodging) 
		{
			Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Path);
		}		
		else
		{
			Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Path_Dodging);
		}
		path.push_back(Astar::getInstance()->mapGrid[it->x][it->y]);
	}

	_isMoving = true;
}

void NPC::Move(D3DXVECTOR2 destination)
{
	if (abs(destination.x - Position.x) < 2)
	{
		destination.x = round(destination.x);
		Position.x = destination.x;
	}

	if (abs(destination.y - Position.y) < 2)
	{
		destination.y = round(destination.y);
		Position.y = destination.y;
	}

	if (destination.x - Position.x < -0.5)
	{
		_direction = D_Left;
		_isMoving = true;
	}
	else if (destination.x - Position.x > 0.5)
	{
		_direction = D_Right;
		_isMoving = true;
	}
	else if (destination.y - Position.y < -0.5)
	{
		_direction = D_Up;
		_isMoving = true;
	}
	else if (destination.y - Position.y > 0.5)
	{
		_direction = D_Down;
		_isMoving = true;
	}
	else
	{
		_direction = D_Stand;
		_isMoving = false;
	}
	
	_timeWaittingMove = 0;

	ApplyAnimation();
	ApplyVelocity();
	PredictCollision();
}

void NPC::Stop() 
{
	_direction = D_Stand;
	_isMoving = false;
	ApplyAnimation();
	ApplyVelocity();
	printLog("Stop");
}

void NPC::SetIsCollision(bool isCollision) {
	_isCollision = isCollision;
	if (_isCollision) Stop();
}


