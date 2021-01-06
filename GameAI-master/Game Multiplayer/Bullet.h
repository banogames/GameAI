#pragma once

#include "Entity.h"
#include "Explosion.h"
#include "GameCollision.h"
#include "Eagle.h"
#include "Astar.h"

class Bullet : public Entity
{
	const float _speed = 400.f;
	Direction _direction;	// hướng bay
	EntityType typePlayer;

	Explosion _explosion = new Explosion(false);
	
	Animation* _leftAnimation;
	Animation* _rightAnimation;
	Animation* _upAnimation;
	Animation* _downAnimation;
	Animation* _currentAnimation; // trỏ đến 1 trong 4 animation để vẽ lên

public:
	Bullet()
	{
		_type = ET_Bullet;
		IsDeleted = true;
		_width = 6;
		_height = 6;

		_leftAnimation = new Animation();
		_leftAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 148, 148 + 8, 12, 12 + 6, D3DXVECTOR2(4.f, 3.f)));
		
		_rightAnimation = new Animation();
		_rightAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 180, 180 + 8, 12, 12 + 6, D3DXVECTOR2(4.f, 3.f)));
		
		_upAnimation = new Animation();
		_upAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 134, 134 + 6, 12, 12 + 8, D3DXVECTOR2(3.f, 4.f)));
		
		_downAnimation = new Animation();
		_downAnimation->addFrameInfo(FrameInfo(SpriteList::Instance()->Others, 166, 166 + 6, 12, 12 + 8, D3DXVECTOR2(3.f, 4.f)));

		_currentAnimation = _leftAnimation;

	}
	
	~Bullet() { }

	void Update(float _dt) override
	{
		_explosion.Update(_dt);

		if (IsDeleted)
			return;
		
		Position += Velocity * _dt;
	}

	void Draw() override
	{
		_explosion.Draw();

		if (IsDeleted)
			return;

		_currentAnimation->Draw(Position);

	}

	void CheckCollision(Entity * entity)
	{
		if (entity->IsDeleted) return;
		if (IsDeleted)
			return;
		CollisionResult cR = GameCollision::getCollisionResult(this, entity);
		if (cR.IsCollided)
		{
			IsDeleted = true;
			_explosion.Spawn(Position);

			//kiểm tra đối tượng va chạm với đạn
			switch (entity->getType())
			{
				//nếu viên đạn của NPC bắn nổ brick => cập nhật lại astar tới điểm lưu
			case ET_NormalBrick:
				entity->DecreasedHeart();
				if (entity->IsDeleted)
				{
					Astar::getInstance()->SetValue(entity->Position.x / X_STEP, entity->Position.y / Y_STEP, 0);
				}
				break;

			case ET_NPC:
				//nếu bắn trúng NPC
				if (typePlayer == ET_Player) 
				{
					entity->DecreasedHeart();
					if (entity->IsDeleted)
					{
						Astar::getInstance()->SetValue(entity->Position.x / X_STEP, entity->Position.y / Y_STEP, 0);
					}
				}
				break;

			case ET_Player:
				//nếu bắn trúng Player
				if (typePlayer == ET_NPC)
				{
					//entity->DecreasedHeart();
					if (entity->IsDeleted)
					{
						Astar::getInstance()->SetValue(entity->Position.x / X_STEP, entity->Position.y / Y_STEP, 0);
					}
				}
				break;

			case ET_EaglePlayer:
				if (typePlayer == ET_NPC)
				{
					entity->DecreasedHeart();
					if (entity->IsDeleted)
					{
						Astar::getInstance()->SetValue(entity->Position.x / X_STEP, entity->Position.y / Y_STEP, 0);
					}
				}
				break;
			case ET_EagleNPC:
				if (typePlayer == ET_Player)
				{
					entity->DecreasedHeart();
					if (entity->IsDeleted)
					{
						Astar::getInstance()->SetValue(entity->Position.x / X_STEP, entity->Position.y / Y_STEP, 0);
					}
				}
				break;
			default:
				break;
			}
		}
	}

	void MakeCollision(Entity* _en) override
	{
		IsDeleted = true;
	}

	void Shoot(D3DXVECTOR2 pos, Direction _dir, EntityType _typePlayer)
	{
		Position = pos;
		setDirection(_dir);
		IsDeleted = false;
		typePlayer = _typePlayer;
		ApplyVelocity();
	}

	// thay đổi vận tốc và animation đựa theo hướng bay
	void setDirection(Direction _dir)
	{
		_direction = _dir;
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
		}
	}

	void ApplyVelocity()
	{
		if (IsDeleted)
			return;

		switch (_direction)
		{
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
		default:
			break;
		}
	}

};


