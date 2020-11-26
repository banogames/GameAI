#pragma once

#include "Entity.h"
#include "Explosion.h"
#include <vector>
#include "GridTile.h"

class NPC :public Entity
{
	const float _speed = 30.f; //130.f;
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
	bool CheckCollision(Entity* entity);
	void ApplyVelocity();
	void addExplosion(Explosion* e) { _explosionList.push_back(e); }
	void SetPosition(float x, float y);

private:
	std::vector<GridTile*> path;
	int currentNodeIndex = 0;
	bool _isMoving = true;
	bool _isDodging = false;  //đang né tránh vật cản trên đường
	bool _isCollision = false; //đang có va chạm hay không

	float _timeWaittingMove = 0; //đếm thời gian
	Vec2* _desSave = new Vec2(); //lưu vị trí cuối path

	//bool _isLeavingCollision = false; //nếu đang rời khỏi va chạm thì chỉ có đi tới điểm đó
	//D3DXVECTOR2 _posLeaveColl = D3DXVECTOR2(); 
	//void MoveNoPredictCollision(D3DXVECTOR2 destination);

	void AutoMove(float dt);
	void Move(D3DXVECTOR2 destination);
	
	void Stop();
	

	//khi hết di chuyển tự động auto lại star
	void RunAstar();
	void PathAstar(int posX, int posY);
	void RunDodging(); //di chuyển né

	//dự đoán va chạm
	void PredictCollision();

	void AutoShoot(); //tự động bắn với tốc độ chậm, => nếu trên đường bắn player thì bắn với tốc độ nhanh
	bool IsPlayerInRange();	//nếu player ở trong vùng quét => astar tới chỗ có thể bắn được player
	//chỗ bắn được player tính bằng trục ngang, trục dọc theo gird player
	//di chuyển tới trục ngang, trục dọc không có vật cản để bắn

public:
	void SetIsCollision(bool isCollision);

	//ở ngoài set đường đi cho bot
	void MovoToGridAstar(GridTile des);
	void SetAutoMove(bool _autoMove);


	void DrawPath();
};

