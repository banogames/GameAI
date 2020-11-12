#pragma once

#include "Entity.h"
#include "Explosion.h"

class NPC :public Entity
{
	const float _speed = 130.f;
	Direction _direction; // hướng di chuyển

	std::vector<Explosion*> _explosionList; // trỏ đến

	Animation* _leftAnimation;
	Animation* _rightAnimation;
	Animation* _upAnimation;
	Animation* _downAnimation;
	Animation *_currentAnimation; // animation hiện tại

	void ApplyAnimation();
	D3DXVECTOR2 getVelocityByDirection(Direction direction);

public:
	NPC();
	~NPC() {}
	void Update(float dt) override;
	void Draw() override;
	void CheckCollision(Entity* entity);
	void ApplyVelocity();
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }
};

