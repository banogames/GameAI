#pragma once

#include "Entity.h"
#include <map>
#include "Bullet.h"
#include <vector>
#include "GameLog.h"
#include "GameDefine.h"

using namespace std;

class Player : public Entity
{
	const float _speed = 130.f;
	Direction _direction = D_Stand;
	bool _isShield = false;
	int _level = 1;
	int _score = 0;
	int _color_Position = 0;

	const float _time_BetweenShoots = 0.7f;
	float _count_Shoot = 0.0f;

	bool _isMoving = false;

	std::vector<Explosion*> _explosionList; // trỏ đến

	Animation* _leftAnimationLv01;
	Animation* _rightAnimationLv01;
	Animation* _upAnimationLv01;
	Animation* _downAnimationLv01;
	Animation* _leftAnimationLv02;
	Animation* _rightAnimationLv02;
	Animation* _upAnimationLv02;
	Animation* _downAnimationLv02;
	Animation* _leftAnimationLv03;
	Animation* _rightAnimationLv03;
	Animation* _upAnimationLv03;
	Animation* _downAnimationLv03;
	Animation* _currentAnimation; // animation hiện tại

	Animation* _arrowAnimation; // animation mũi tên ở trên người chơi
	Animation* _shieldAnimation; // animation khiên 
	Animation* _spawnAnimation; // animation hồi sinh

	void SetAnimation(Direction _dir);
	void InitAnimation();
	void LogPosition() { GAMELOG("(%i, %i)", (int)Position.x, (int)Position.y); }	
public:
	Player();
	~Player() {}
	void Update(float dt);
	void Draw();
	void DrawArrow();
	void CheckCollision(Entity* entity);
	void HandleKeyboard(std::map<int, bool> _keys, float _dt);
	void ApplyVelocity(); // vận tốc theo direction
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }
	int getScore() { return _score; }

	bool IsMoving() { return _isMoving; }
	void Move(D3DXVECTOR2 destination);
	void Stop(D3DXVECTOR2 preDestination, D3DXVECTOR2 currentDestination);
};