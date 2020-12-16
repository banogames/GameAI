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

	// tạo 6 bullet cho player
	for (int i = 0; i < 6; i++)
	{
		Bullet* bullet = new Bullet();
		_bulletList.push_back(bullet);
	}

	_isAutoShoot = true;
}

void NPC::SetPosition(float x, float y) 
{
	Position = D3DXVECTOR2(x, y);
}

void NPC::Update(float _dt)
{
	for (auto bullet : _bulletList)
	{
		bullet->Update(_dt);
	}

	if (IsDeleted)
		return;

	Position += Velocity * _dt;

	AutoMove(_dt);

	AutoShoot(_dt);
}

void NPC::Draw()
{
	if (!_bulletList.empty())
	{
		for (auto bullet : _bulletList)
		{
			bullet->Draw();
		}
	}

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
	//check collision bullet
	for (auto bullet : _bulletList)
	{
		bullet->CheckCollision(e);
	}

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
			Position.x += (float)(cR.Rect.right - cR.Rect.left) + 1;
		}
		else if (cR.Side == CS_Right)
		{
			Position.x -= (float)(cR.Rect.right - cR.Rect.left) - 1;
		}
		else if (cR.Side == CS_Top)
		{
			Position.y += (float)(cR.Rect.bottom - cR.Rect.top) + 1;
		}
		else if (cR.Side == CS_Bottom)
		{
			Position.y -= (float)(cR.Rect.bottom - cR.Rect.top) - 1;
		}

		//Astar::getInstance()->SetValue(Position.x / X_STEP, Position.y / Y_STEP, 2);


		//kiểm tra node hiện tại, node trước => valid sẽ tốc biến về đó
		if (!path.empty())
		{
			for (int i = -1; i <= 0; i++)
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

template < typename T>
std::pair<bool, int > findInVector(const std::vector<T>  & vecOfElements, const T  & element)
{
	std::pair<bool, int > result;
	// Find given element in vector
	auto it = std::find(vecOfElements.begin(), vecOfElements.end(), element);
	if (it != vecOfElements.end())
	{
		result.second = distance(vecOfElements.begin(), it);
		result.first = true;
	}
	else
	{
		result.first = false;
		result.second = -1;
	}
	return result;
}

void NPC::AutoMove(float dt)
{
	if (_isCollision)
	{
		_timeWaittingMove += dt;
		if (_timeWaittingMove > 3) //đợi 3s rời va chạm
		{
			_timeWaittingMove = 0;
			RunDodging();
			return;
		}
		return;
	}

	if (_stateRun == S_Attacking)
	{
		_timeWaittingMove += dt;
		if (_timeWaittingMove > 2) //đợi 2s bắn
		{
			_timeWaittingMove = 0;
			_stateRun = S_RunAstar;
			return;
		}
		return;
	}

	if (_isPlayerInRange && _stateRun != S_RunAttack)
	{
		int posX, posY;
		posX = (Position.x / X_STEP);
		posY = (Position.y / Y_STEP);

		if (!path.empty()) 
		{
			//xóa node cũ
			if (currentNodeIndex < path.size())
			{
				Astar::getInstance()->SetValue(path.at(currentNodeIndex)->GetVec().x, path.at(currentNodeIndex)->GetVec().y, 0);
			}
		}
		
		_stateRun = S_RunAttack;

		//sort từ gần đến xa NPC (ưu tiên đi tới điểm gần trước)
		int xRound = posX < vecPlayer->x ? posX - _rangeAttack : posX + _rangeAttack;
		int yRound = posY < vecPlayer->y ? posY - _rangeAttack : posY + _rangeAttack;
		std::vector<Vec2*> validList = Astar::getInstance()->GetListVecInAxisValid(vecPlayer->x, vecPlayer->y, xRound, yRound);
		Vec2 vecAttack;
		if (!validList.empty())
		{
			//sort lại theo list có khoảng cách nhỏ nhất (buble sort)
			for (int i = 0; i < validList.size(); i++) 
			{
				// Last i elements are already in place  
				for (int j = 0; j < validList.size() - i - 1; j++)
				{
					if (vec->distance(*validList.at(i)) < vec->distance(*validList.at(j)))
					{
						//thay đổi giá trị
						Vec2 vecTemp;
						vecTemp.x = validList.at(i)->x;
						vecTemp.y = validList.at(i)->y;

						validList.at(i)->x = validList.at(j)->x;
						validList.at(i)->y = validList.at(j)->y;

						validList.at(j)->x = vecTemp.x;
						validList.at(j)->y = vecTemp.y;
					}
				}		
			}
				
			for (auto vec : validList)
			{
				PathAstar(vec->x, vec->y);
				if (!path.empty())
				{
					printLog("Move attack");
					vecAttack.x = vec->x;
					vecAttack.y = vec->y;		
					break;
				}
			}

			//xoay mặt về hướng player, lưu hướng bắn
			if (vecPlayer->x == vecAttack.x)
			{
				_directionAttack = vecPlayer->y > vecAttack.y ? D_Down : D_Up;
			}
			else if (vecPlayer->y == vecAttack.y)
			{
				_directionAttack = vecPlayer->x > vecAttack.x ? D_Right : D_Left;
			}
			else
			{
				_directionAttack = _direction;
			}
		}

	}
	
	if (!path.empty())
	{
		if (currentNodeIndex >= path.size())
		{
			if (_stateRun != S_RunAttack)
			{
				//bỏ giá trị node cuối
				Astar::getInstance()->SetValue(path[path.size() -1]->GetVec().x, path[path.size() - 1]->GetVec().y, 0);

				RunAstar();
			}
			else
			{
				_stateRun = S_Attacking; //đứng lại bắn player
				_direction = _directionAttack;
				_directionBullet = _directionAttack;
				ApplyAnimation();
				Stop();
			}
		}
		else
		{			
			Move(path[currentNodeIndex]->GetPosition());
			if (!_isMoving) 
			{
				//bỏ giá trị node đuôi
				Astar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x, path[currentNodeIndex]->GetVec().y, 0);
				
				//tăng node
				currentNodeIndex++;

				//set giá trị node mới
				if (currentNodeIndex < path.size())
					Astar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x, path[currentNodeIndex]->GetVec().y, 2);
			}
		}
	}
	else
	{
		if (_stateRun != S_RunAttack) 
		{
			RunAstar();
		}
		else
		{
			_stateRun = S_Attacking; //đứng lại bắn player
				//xoay mặt về hướng player
			_direction = _directionAttack;
			_directionBullet = _directionAttack;
			ApplyAnimation();
			Stop();
		}
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
					if (Astar::getInstance()->isObstacle(path.at(i)->GetVec().x, path.at(i)->GetVec().y, Astar::getInstance()->map))
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

	//nếu còn va chạm kiểm tra 9 node xung quanh, node nào có thể tới => tốc biến tới node đó (node không có va chạm)
	if (_isCollision) 
	{
		Vec2 *pos = new Vec2();
		if (Astar::getInstance()->RandomoPosValidAround(pos0, pos, 1))
		{
			Position.x = (pos->x + 0.5)* X_STEP;
			Position.y = (pos->y + 0.5)* Y_STEP;
		}
	}
		
	//random 1 node xung quanh, bán kính 4 ô
	if (Astar::getInstance()->RandomoPosValidAround(pos0, pos, 4)) {
		//random path xung quanh

		//lưu node cuối lại, sau khi end path né => astar tới node cuối
		if (!_isDodging) 
		{
			if (!path.empty()) 
			{
				_desSave->x = (path.back())->GetVec().x;
				_desSave->y = (path.back())->GetVec().y;
			}
		}
		_isDodging = true;		
		_stateRun = S_RunDodging;
		PathAstar(pos->x, pos->y);
	}
}


void NPC::RunAstar() 
{
	_stateRun = S_RunAstar;
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
		if (IS_DRAW_PATH_ASTAR) 
		{
			switch (_stateRun)
			{
			case S_RunAstar:
				Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Path);
				break;
			case S_RunDodging:
				Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Path_Dodging);
				break;
			case S_RunAttack:
				Astar::getInstance()->mapGrid[it->x][it->y]->SetType(Path_Attacking);
				break;
			}
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
		_directionBullet = D_Left;
		_isMoving = true;
	}
	else if (destination.x - Position.x > 0.5)
	{
		_direction = D_Right;
		_directionBullet = D_Right;
		_isMoving = true;
	}
	else if (destination.y - Position.y < -0.5)
	{
		_direction = D_Up;
		_directionBullet = D_Up;
		_isMoving = true;
	}
	else if (destination.y - Position.y > 0.5)
	{
		_direction = D_Down;
		_directionBullet = D_Down;
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

	//cập nhật vec
	vec->x = Position.x / X_STEP;
	vec->y = Position.y / Y_STEP;
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

void NPC::MoveGridAstar(int x, int y)
{
	PathAstar(x, y);
}

#pragma region  SHOOT

void NPC::AutoShoot(float dt) 
{
	if (!_isAutoShoot) return;

	_timeWaittingShoot += dt;
	float _timeDelay = (_stateRun == S_RunAttack || _stateRun == S_Attacking) ? _timeDelayShoot/2 : _timeDelayShoot;
	if (_timeWaittingShoot > _timeDelay)
	{
		_timeWaittingShoot = 0;
		//shot
		//shoot bullet
		if (!_bulletList.empty() && _directionBullet != D_Stand)
		{
			printLog("Shoot bullet");
			if (_currentBullet >= _bulletList.size())
				_currentBullet = 0;
			_bulletList.at(_currentBullet)->Shoot(Position, _directionBullet);
			_currentBullet++;
		}
	}
}

void NPC::CheckPlayerInRange(int x, int y) 
{
	//kiểm tra player trong tầm bắn
	int posX, posY;
	posX = (Position.x / X_STEP);
	posY = (Position.y / Y_STEP);

	int left, right, top, bottom;

	if (x <= posX + _rangeAttack && x >= posX - _rangeAttack && y <= posY + _rangeAttack && y >= posY - _rangeAttack) {
		_isPlayerInRange = true;

		vecPlayer->x = x;
		vecPlayer->y = y;
	}
	else
	{
		_isPlayerInRange = false;
	}

}

#pragma endregion


