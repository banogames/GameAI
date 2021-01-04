#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>
#include "Entity.h"

class GraphicsDevice;

class Camera
{
public:
	//Constructors and Destructor
	Camera(GraphicsDevice* gDevice, int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors);
	~Camera();

	//Game Functions
	void Update();
	void Follow(Entity *following);
	void Unfollow();
	bool IsFollowing() const;
	void SetTransform(GraphicsDevice *gDevice);
	void Draw();

private:
	float angle;
	DirectX::XMFLOAT3 scaleFactors;
	D3DXMATRIX orthographicMatrix;
	D3DXMATRIX identityMatrix;
	D3DXMATRIX viewMatrix;

	Entity *following;
	GraphicsDevice* _gDevice;

	
public:
	int width;
	int height;

	int posX;
	int posY;
};

#endif /* CAMERA_H */
