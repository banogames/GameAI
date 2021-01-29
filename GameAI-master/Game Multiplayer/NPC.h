#pragma once

#include "Entity.h"
#include "Explosion.h"
#include <vector>
#include "GridTile.h"
#include "Bullet.h"
#include "Label.h"
#include <algorithm> 

enum NPCState
{
	S_RunAstar,
	S_RunDodging,
	S_RunAttack,
	S_Attacking,
	S_RunCollect,
	S_RunNoAttack
};

class NPC :public Entity
{
protected:
	float _speed = 100.f; //130.f;
	int _maxHeart = 5; 
	bool _isShield = false;
	Direction _direction; // hướng di chuyển
	Direction _directionAttack = D_Stand; // lưu hướng tấn công
	Direction _directionBullet = D_Stand; //lưu hướng bắn đạn
	NPCState _stateRun = S_RunAstar; //lưu hướng bắn đạn

	std::vector<Explosion*> _explosionList; // trỏ đến
	

	Animation* _leftAnimation;
	Animation* _rightAnimation;
	Animation* _upAnimation;
	Animation* _downAnimation;
	Animation *_currentAnimation; // animation hiện tại
	Animation* _shieldAnimation; // animation khiên 

	//Label _heatLabel;

	void ApplyAnimation();
	D3DXVECTOR2 getVelocityByDirection(Direction direction);

public:
	std::vector<Bullet*> _bulletList;

	NPC();
	~NPC() {}
	virtual void Init();
	void Update(float dt) override;
	void Draw() override;
	bool CheckCollision(Entity* entity);
	void ApplyVelocity();
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }
	void SetPosition(float x, float y);

protected:
	//tọa độ trên astar
	Vec2* vec = new Vec2();

	std::vector<GridTile*> path;
	int currentNodeIndex = 0;
	bool _isMoving = true;
	bool _isDodging = false;  //đang né tránh vật cản trên đường
	bool _isCollision = false; //đang có va chạm hay không

	float _timeWaittingMove = 0; //đếm thời gian
	Vec2* _desSave = new Vec2(); //lưu vị trí cuối path

	float _time_Shield = 0.f;

	void AutoMove(float dt);
	void Move(D3DXVECTOR2 destination);
	
	void Stop();
	

	//khi hết di chuyển tự động auto lại star
	void RunAstar();
	void PathAstar(int posX, int posY);
	void RunDodging(); //di chuyển né
	virtual void RunRandom();

	//dự đoán va chạm
	void PredictCollision();


	//SHOOT
	bool _isAutoShoot;
	float  _timeWaittingShoot;
	float _timeDelayShoot = 4; //delay 4s để bắn
	int _currentBullet;
	int _rangeAttack =8;
	int _ratioAttack = 100; //50% sẽ đi bắn (tỷ lệ bắn mỗi lần quét)
	bool _isPlayerInRange; //player trong tầm nhắm thì đuổi theo bắn player
	Vec2* vecPlayer = new Vec2();

	void AutoShoot(float dt); //tự động bắn với tốc độ chậm, => nếu trên đường bắn player thì bắn với tốc độ nhanh

public:
	void SetIsCollision(bool isCollision);

	//ở ngoài set đường đi cho bot
	void MoveGridAstar(int x, int y);

	bool CheckPlayerInRange(int x, int y, bool isCollect = false);
	//nếu player ở trong vùng quét => astar tới chỗ có thể bắn được player
	//chỗ bắn được player tính bằng trục ngang, trục dọc theo gird player
	//di chuyển tới trục ngang, trục dọc không có vật cản để bắn

	void DrawPath();

	void DecreasedHeart(int _value = 1) override
	{
		if (!_isShield)
		{
			Entity::DecreasedHeart(_value);
		}
	}
};



