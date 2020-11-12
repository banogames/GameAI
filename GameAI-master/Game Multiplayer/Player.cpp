#include <math.h>
#include "Player.h"

#include "GameLog.h"
#include "SpriteList.h"
#include "GameCollision.h"

Player::Player()
{
	_type = ET_Player;
	InitAnimation();
	_currentAnimation = _upAnimationLv01;
	_width = 32;
	_height = 32;

	// set vị trí ban đầu & màu người chơi
	switch (_color_Position)
	{
	case 0:
		Position = D3DXVECTOR2(55.f, 55.f);
		break;
	case 1:
		Position = D3DXVECTOR2(745.f, 55.f);
		break;
	case 2:
		Position = D3DXVECTOR2(55.f, 745.f);
		break;
	case 3:
		Position = D3DXVECTOR2(745.f, 745.f);
		break;
	default:
		break;
	}
}

void Player::Update(float _dt)
{
	if (IsDeleted)
		return;

	Position += Velocity * _dt;

	if (_isShield)
		_shieldAnimation->Update(_dt);
}

void Player::HandleKeyboard(std::map<int, bool> keys, float _dt)
{
	if (IsDeleted)
		return;

	if (_count_Shoot >= 0)
	{
		_count_Shoot -= _dt;
	}
	if (_count_Shoot < 0 && keys[VK_SPACE]) // gửi data bắn đạn
	{
		_count_Shoot += _time_BetweenShoots;
		
		// shoot
	}

	if (keys[VK_LEFT])
	{
		_direction = D_Left;
	}
	else if (keys[VK_RIGHT])
	{
		_direction = D_Right;
	}
	else if (keys[VK_UP])
	{
		_direction = D_Up;
	}
	else if (keys[VK_DOWN])
	{
		_direction = D_Down;
	}
	else
	{
		_direction = D_Stand;
	}

	SetAnimation(_direction);
	ApplyVelocity();
}

void Player::Move(D3DXVECTOR2 destination)
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
		
	if (destination.x - Position.x < 0)
	{
		_direction = D_Left;
		_isMoving = true;
	}
	else if (destination.x - Position.x > 0)
	{
		_direction = D_Right;
		_isMoving = true;
	}
	else if (destination.y - Position.y < 0) 
	{
		_direction = D_Up;
		_isMoving = true;
	}
	else if (destination.y - Position.y > 0)
	{
		_direction = D_Down;
		_isMoving = true;
	}
	else
	{
		_direction = D_Stand;
		_isMoving = false;
	}

	SetAnimation(_direction);
	ApplyVelocity();
}

void Player::Stop(D3DXVECTOR2 preDestination, D3DXVECTOR2 currentDestination)
{
	_direction = D_Stand;
	/*if (abs(currentDestination.x - Position.x) < 2)
	{
		currentDestination.x = round(currentDestination.x);
		Position.x = currentDestination.x;
	}
	else
	{
		preDestination.x = round(preDestination.x);
		Position.x = preDestination.x;
	}

	if (abs(currentDestination.y - Position.y) < 2)
	{
		currentDestination.y = round(currentDestination.y);
		Position.y = currentDestination.y;
	}
	else
	{
		preDestination.y = round(preDestination.y);
		Position.y = preDestination.y;
	}*/
	_isMoving = false;
	SetAnimation(_direction);
	ApplyVelocity();
}

void Player::SetAnimation(Direction direction)
{
	if (_level == 1)
	{
		switch (direction)
		{
		case D_Left:
			_currentAnimation = _leftAnimationLv01;
			break;
		case D_Right:
			_currentAnimation = _rightAnimationLv01;
			break;
		case D_Up:
			_currentAnimation = _upAnimationLv01;
			break;
		case D_Down:
			_currentAnimation = _downAnimationLv01;
			break;
		default:
			break;
		}
	}
	else if (_level == 2)
	{
		switch (direction)
		{
		case D_Left:
			_currentAnimation = _leftAnimationLv02;
			break;
		case D_Right:
			_currentAnimation = _rightAnimationLv02;
			break;
		case D_Up:
			_currentAnimation = _upAnimationLv02;
			break;
		case D_Down:
			_currentAnimation = _downAnimationLv02;
			break;
		default:
			break;
		}
	}
	else if (_level == 3)
	{
		switch (direction)
		{
		case D_Left:
			_currentAnimation = _leftAnimationLv03;
			break;
		case D_Right:
			_currentAnimation = _rightAnimationLv03;
			break;
		case D_Up:
			_currentAnimation = _upAnimationLv03;
			break;
		case D_Down:
			_currentAnimation = _downAnimationLv03;
			break;
		default:
			break;
		}
	}
}

void Player::InitAnimation()
{
	_leftAnimationLv01 = new Animation();
	_rightAnimationLv01 = new Animation();
	_upAnimationLv01 = new Animation();
	_downAnimationLv01 = new Animation();
	_leftAnimationLv02 = new Animation();
	_rightAnimationLv02 = new Animation();
	_upAnimationLv02 = new Animation();
	_downAnimationLv02 = new Animation();
	_leftAnimationLv03 = new Animation();
	_rightAnimationLv03 = new Animation();
	_upAnimationLv03 = new Animation();
	_downAnimationLv03 = new Animation();

	// animation với màu xe tăng của người chơi
	if (_color_Position == 0)
	{
		_leftAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
		_rightAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
		_upAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 4, 4 + 32, D3DXVECTOR2(16.f, 16.f)));
		_downAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));

		_leftAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 76, 76 + 37, 196, 196 + 32, D3DXVECTOR2(18.f, 16.f)));
		_rightAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 195, 195 + 30, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 192, 192 + 36, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 195, 195 + 34, D3DXVECTOR2(15.f, 17.f)));

		_leftAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 77, 77 + 37, 79, 79 + 31, D3DXVECTOR2(18.f, 15.f)));
		_rightAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 79, 79 + 31, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 77, 77 + 37, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 78, 78 + 37, D3DXVECTOR2(15.f, 18.f)));
	}
	else if (_color_Position == 1)
	{
		_leftAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
		_rightAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));
		_upAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 4, 4 + 32, D3DXVECTOR2(16.f, 16.f)));
		_downAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 2, 2 + 32, D3DXVECTOR2(16.f, 16.f)));

		_leftAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 196, 196 + 32, D3DXVECTOR2(18.f, 16.f)));
		_rightAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 195, 195 + 30, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 192, 192 + 36, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 195, 195 + 34, D3DXVECTOR2(15.f, 17.f)));

		_leftAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 79, 79 + 31, D3DXVECTOR2(18.f, 15.f)));
		_rightAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 79, 79 + 31, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 77, 77 + 37, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 78, 78 + 37, D3DXVECTOR2(15.f, 18.f)));
	}
	else if (_color_Position == 2)
	{
		_leftAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 82, 82 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		_rightAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		_upAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 312, 312 + 32, D3DXVECTOR2(16.f, 16.f)));
		_downAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));

		_leftAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 76, 76 + 37, 504, 504 + 32, D3DXVECTOR2(18.f, 16.f)));
		_rightAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 502, 502 + 30, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 500, 500 + 36, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 502, 502 + 34, D3DXVECTOR2(15.f, 17.f)));

		_leftAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 77, 77 + 37, 387, 387 + 31, D3DXVECTOR2(18.f, 15.f)));
		_rightAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 233, 233 + 36, 387, 387 + 31, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 2, 2 + 32, 384, 384 + 37, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 156, 156 + 31, 387, 387 + 37, D3DXVECTOR2(15.f, 18.f)));
	}
	else if (_color_Position == 3)
	{
		_leftAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 389, 389 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		_rightAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));
		_upAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 312, 312 + 32, D3DXVECTOR2(16.f, 16.f)));
		_downAnimationLv01->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 32, 310, 310 + 32, D3DXVECTOR2(16.f, 16.f)));

		_leftAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 504, 504 + 32, D3DXVECTOR2(18.f, 16.f)));
		_rightAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 502, 502 + 30, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 500, 500 + 36, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv02->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 502, 502 + 34, D3DXVECTOR2(15.f, 17.f)));

		_leftAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 384, 384 + 37, 387, 387 + 31, D3DXVECTOR2(18.f, 15.f)));
		_rightAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 540, 540 + 36, 387, 387 + 31, D3DXVECTOR2(18.f, 15.f)));
		_upAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 310, 310 + 32, 384, 384 + 37, D3DXVECTOR2(16.f, 18.f)));
		_downAnimationLv03->addFrameInfo(FrameInfo(SpriteList::Instance()->Tank, 464, 464 + 31, 387, 387 + 37, D3DXVECTOR2(15.f, 18.f)));
	}

	_arrowAnimation = new Animation();
	_arrowAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Arrow, 0, 26, 0, 26, D3DXVECTOR2(13.f, 13.f)));

	_shieldAnimation = new Animation(0.07f);
	_shieldAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 0 + 39, 111, 111 + 40, D3DXVECTOR2(20.f, 20.f)));
	_shieldAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 39, 39 + 40, 111, 111 + 40, D3DXVECTOR2(20.f, 20.f)));

	_spawnAnimation = new Animation(0.1f);
	_spawnAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 0, 30, 0, 30, D3DXVECTOR2(15.f, 15.f)));
	_spawnAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 30, 60, 0, 30, D3DXVECTOR2(15.f, 15.f)));
	_spawnAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 60, 60 + 34, 0, 0 + 31, D3DXVECTOR2(17.f, 15.f)));
	_spawnAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 96, 93 + 35, 0, 0 + 32, D3DXVECTOR2(17.f, 16.f)));
}

void Player::ApplyVelocity()
{
	switch (_direction)
	{
	case D_Stand:
		Velocity = D3DXVECTOR2(0, 0);
		break;
	case D_Left:
		Velocity = D3DXVECTOR2(-_speed, 0);
		break;
	case D_Right:
		Velocity = D3DXVECTOR2(_speed, 0);
		break;
	case D_Up:
		Velocity = D3DXVECTOR2(0, -_speed);
		break;
	case D_Down:
		Velocity = D3DXVECTOR2(0, _speed);
		break;
	}
}

void Player::Draw()
{
	if (!IsDeleted)
	{
		_currentAnimation->Draw(Position);
		if (_isShield)
			_shieldAnimation->Draw(Position);
	}
}

void Player::DrawArrow()
{
	if (IsDeleted)
	{
		_arrowAnimation->setScale(D3DXVECTOR2(0.66f, 0.66f));
		_arrowAnimation->Draw(Position + D3DXVECTOR2(0, -25.0f));
	}
}

void Player::CheckCollision(Entity * entity)
{
	if (IsDeleted)
		return;

	CollisionResult cR = GameCollision::getCollisionResult(this, entity);
	if (cR.IsCollided)
	{
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
	}
}
