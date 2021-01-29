#include "NPC.h"

#include "SpriteList.h"
#include "GameCollision.h"
#include "GameDefine.h"
#include "Astar.h"

NPC::NPC()
{
	Init();
}

void NPC::Init() 
{
	IsDeleted = false;
	_width = 25;
	_height = 25;

	//khởi tạo các animation
	_leftAnimation = new Animation();
	_rightAnimation = new Animation();
	_upAnimation = new Animation();
	_downAnimation = new Animation();


	_shieldAnimation = new Animation(0.07f);
	_shieldAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 39, 111, 111 + 40, D3DXVECTOR2(20.f, 20.f)));
	_shieldAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 39, 39 + 40, 111, 111 + 40, D3DXVECTOR2(20.f, 20.f)));

	// mặc định animation ban đầu
	_currentAnimation = _leftAnimation;
	Position = D3DXVECTOR2(300.f, 300.f);
	_isMoving = true;
	_isShield = false;

	// tạo 6 bullet cho player
	for (int i = 0; i < 4; i++)
	{
		Bullet* bullet = new Bullet();
		_bulletList.push_back(bullet);
	}

	_isAutoShoot = true;
	//_heatLabel = Label(to_string(_heart), 20, 15, Position);

}

void NPC::SetPosition(float x, float y) 
{
	Position = D3DXVECTOR2(x, y);

	Astar::getInstance()->SetValue(x / X_STEP, y / Y_STEP, VALUE_ASTAR_NPC);
}

void NPC::Update(float _dt)
{
	if (IsDeleted)
		return;

	if (!_isCollision)
		Position += Velocity * _dt;

	AutoMove(_dt);

	AutoShoot(_dt);

	if (_isShield)
	{
		_shieldAnimation->Update(_dt);

		_time_Shield += _dt;
		if (_time_Shield > TIME_SHIELD) {
			_isShield = false;
			_time_Shield = 0;
		}
	}
}

void NPC::DrawPath() 
{
	if (IS_DRAW_PATH_ASTAR && !path.empty())
	{
		for (auto grid : path)
		{
			grid->Draw();
		}

		//draw des
		Astar::getInstance()->mapGrid[_desSave->x][_desSave->y]->Draw();
	}
}

void NPC::Draw()
{
	if (IsDeleted)
		return;

	_currentAnimation->Draw(Position);

	if (_isShield)
		_shieldAnimation->Draw(Position);

	//_heatLabel.setPosition(Position);
	//_heatLabel.Draw(to_string(_heart), D3DCOLOR_XRGB(0, 255, 0));
}

bool NPC::CheckCollision(Entity * e)
{
	if (e->IsDeleted) return false;

	//check collision bullet
	for (auto bullet : _bulletList)
	{
		bullet->CheckCollision(e);
	}

	if (IsDeleted)
		return false;

	//kiểm tra vị trí không thể va chạm khỏi xét
	if (!CanCollision(e)) return false;


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
		if (!_isCollision && !_isDodging)
		{
			RunDodging(); //chạy né va chạm		
		}

		if (e->getType() == ET_ProtectItem)
		{
			//ăn item bảo vệ
			_isShield = true;
			e->IsDeleted = true;
		}
		else if (e->getType() == ET_HealItem)
		{
			//ăn item máu
			_heart += e->_heart;
			if (_heart > _maxHeart) _heart = _maxHeart;
			e->IsDeleted = true;
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
		Stop();
		_timeWaittingMove += dt;
		if (_timeWaittingMove > 2) //đợi 2s rời va chạm
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

		if (_stateRun == S_RunCollect) 
		{
			_stateRun = S_RunAttack;
			//đi tới item
			PathAstar(vecPlayer->x, vecPlayer->y);
		}
		else {
			_stateRun = S_RunAttack;
			//sort từ gần đến xa NPC (ưu tiên đi tới điểm gần)
			int xRound = posX < vecPlayer->x ? posX - _rangeAttack : posX + _rangeAttack;
			int yRound = posY < vecPlayer->y ? posY - _rangeAttack : posY + _rangeAttack;
			std::vector<Vec2> validList = Astar::getInstance()->GetListVecInAxisValid(vecPlayer->x, vecPlayer->y, xRound, yRound, 0);
			Vec2 vecAttack;
			if (!validList.empty())
			{
				//sort lại theo list có khoảng cách nhỏ nhất (buble sort)
				for (int i = 0; i < validList.size(); i++)
				{
					// Last i elements are already in place  
					for (int j = 0; j < validList.size() - i - 1; j++)
					{
						if (vec->distance(validList.at(i)) < vec->distance(validList.at(j)))
						{
							//thay đổi giá trị
							Vec2 vecTemp;
							vecTemp.x = validList.at(i).x;
							vecTemp.y = validList.at(i).y;

							validList.at(i).x = validList.at(j).x;
							validList.at(i).y = validList.at(j).y;

							validList.at(j).x = vecTemp.x;
							validList.at(j).y = vecTemp.y;
						}
					}
				}

				for (auto vec : validList)
				{
					PathAstar(vec.x, vec.y);
					if (!path.empty())
					{
						printLog("Move attack");
						vecAttack.x = vec.x;
						vecAttack.y = vec.y;
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
			}

		}
		
	}
	
	if (!path.empty())
	{
		if (currentNodeIndex >= path.size())
		{
			if (_stateRun != S_RunAttack)
			{
				RunAstar();
				//_ = false;
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
					Astar::getInstance()->SetValue(path[currentNodeIndex]->GetVec().x, path[currentNodeIndex]->GetVec().y, VALUE_ASTAR_NPC);
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
	if (_isMoving && !_isCollision && !_isDodging)
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
	Stop();

	Vec2 *pos = new Vec2();
	Vec2 pos0;
	pos0.x = Position.x / X_STEP;
	pos0.y = Position.y / Y_STEP;

	//nếu còn va chạm kiểm tra 9 node xung quanh, node nào có thể tới => tốc biến tới node đó (node không có va chạm)
	if (_isCollision) 
	{
		Vec2 *pos = new Vec2();
		if (Astar::getInstance()->RandomoPosValidAround(pos0, pos, 1))
		{
			//tốc biến tách va chạm
			SetPosition((float)(pos->x + 0.5f)* X_STEP, (float)(pos->y + 0.5f)* Y_STEP);
		}
	}
		
	//random 1 node xung quanh, bán kính 3 ô
	if (Astar::getInstance()->RandomoPosValidAround(pos0, pos, 3)) {
		//random path xung quanh

		_isDodging = true;		
		_stateRun = S_RunDodging;
		PathAstar(pos->x, pos->y);
	}
}

void NPC::RunRandom() 
{
	Vec2 *pos = new Vec2();
	if (Astar::getInstance()->RandomPosValid(pos))
	{
		PathAstar(pos->x, pos->y);
	}
}

void NPC::RunAstar() 
{
	_stateRun = S_RunAstar;
	if (!_isDodging) //đang không phải né
	{
		//tiếp tục đi con đường dang dở :D
		if (!path.empty()) 
		{
			if (currentNodeIndex >= path.size())
			{
				if (path.at(path.size() - 1)->GetVec().x == _desSave->x
					&& path.at(path.size() - 1)->GetVec().y == _desSave->y) 
				{
					RunRandom();		
				}
				else
				{
					PathAstar(_desSave->x, _desSave->y);
				}
			}
			else 
			{
				if (path.at(currentNodeIndex)->GetVec().x == _desSave->x
					&& path.at(currentNodeIndex)->GetVec().y == _desSave->y)
				{	
					RunRandom();
				}
				else
				{
					PathAstar(_desSave->x, _desSave->y);
				}
			}
		}
		else //random điểm mới
		{
			RunRandom();
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

	if (_stateRun == S_RunAstar) 
	{
		if (IS_DRAW_PATH_ASTAR)
			Astar::getInstance()->mapGrid[_desSave->x][_desSave->y]->SetType(Empty);

		 //lưu lại điểm cuối
		_desSave->x = posX;
		_desSave->y = posY;

		if (IS_DRAW_PATH_ASTAR)
			Astar::getInstance()->mapGrid[_desSave->x][_desSave->y]->SetType(Destination);
	}
	
	AstarResuit resuit = Astar::getInstance()->findPathV2(beginNode, destNode);

	//draw closed, opened list

	/*for (auto node :resuit.closeList)
	{
		Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Closed);
	}

	for (auto node : resuit.openList)
	{
		Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Opened);
	}*/

	//xóa đường vẽ path cũ
	if (IS_DRAW_PATH_ASTAR && !path.empty()) 
	{
		for (auto grid : path) 
		{
			grid->SetType(Empty);
		}
	}

	//set path
	path.clear();

	for (auto node : resuit.path)
	{
		if (IS_DRAW_PATH_ASTAR) 
		{
			switch (_stateRun)
			{
			case S_RunAstar:
				if (Astar::getInstance()->mapGrid[node.x][node.y]->GetType()!= Destination)
					Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Path);
				break;
			case S_RunDodging:
				if (Astar::getInstance()->mapGrid[node.x][node.y]->GetType() != Destination)
					Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Path_Dodging);
				break;
			case S_RunAttack:
				if (Astar::getInstance()->mapGrid[node.x][node.y]->GetType() != Destination)
					Astar::getInstance()->mapGrid[node.x][node.y]->SetType(Path_Attacking);
				break;
			}
		}

		path.push_back(Astar::getInstance()->mapGrid[node.x][node.y]);
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
	if (!_isCollision && isCollision) 
	{
		Stop();
		RunDodging();	
	}
	_isCollision = isCollision;
}

void NPC::MoveGridAstar(int x, int y)
{
	_desSave->x = x;
	_desSave->y = y;

	printLog(" Move to player");
	_stateRun = S_RunAstar;
	PathAstar(_desSave->x, _desSave->y);
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
			if (_currentBullet >= _bulletList.size())
				_currentBullet = 0;
			_bulletList.at(_currentBullet)->Shoot(Position, _directionBullet , _type);
			_currentBullet++;
		}
	}
}

bool NPC::CheckPlayerInRange(int x, int y, bool isCollect) 
{
	if (_stateRun != S_RunNoAttack) 
	{
		//kiểm tra player trong tầm bắn
		int posX, posY;
		posX = (Position.x / X_STEP);
		posY = (Position.y / Y_STEP);

		int left, right, top, bottom;

		if (x <= posX + _rangeAttack && x >= posX - _rangeAttack && y <= posY + _rangeAttack && y >= posY - _rangeAttack) {
			if (rand() % 100 < _ratioAttack)
			{
				_isPlayerInRange = true;
				if (isCollect) _stateRun = S_RunCollect;
				vecPlayer->x = x;
				vecPlayer->y = y;
			}
			else {
				_stateRun = S_RunNoAttack;
			}
		}
		else
		{
			_isPlayerInRange = false;
		}
		return _isPlayerInRange;
	}
}

#pragma endregion


