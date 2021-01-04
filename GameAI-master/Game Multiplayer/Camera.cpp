#include "Camera.h"
#include "GraphicsDevice.h"
#include "GameDefine.h"

Camera::Camera(GraphicsDevice* gDevice, int width, int height, float angle, DirectX::XMFLOAT3 scaleFactors)
{
	this->_gDevice = gDevice;
	this->width = width;
	this->height = height;
	this->angle = angle;
	this->scaleFactors = scaleFactors;

	D3DXMatrixOrthoLH(&orthographicMatrix, width, -height, 0.0f, 1.0f);
	D3DXMatrixIdentity(&identityMatrix);
}

Camera::~Camera()
{

}

void Camera::Update()
{
	posX = this->width / 2;
	posY = this->height / 2;
	if (this->following) //&& this->following->IsInitialized()
	{
		//không vượt ra khỏi map
		if (this->following->Position.x > this->width / 2)
		{
			posX = this->following->Position.x < (SIZE_MAP_X - this->width / 2)? this->following->Position.x: (SIZE_MAP_X - this->width / 2);
		}
		if (this->following->Position.y > this->height / 2)
		{
			posY = this->following->Position.y < (SIZE_MAP_Y - this->height / 2) ? this->following->Position.y : (SIZE_MAP_Y - this->height / 2);
		}
	}

	this->viewMatrix = D3DXMATRIX(
		scaleFactors.x * cos(angle), scaleFactors.x * sin(angle), 0, 0,
		-scaleFactors.y * sin(angle), scaleFactors.y * cos(angle), 0, 0,
		0, 0, scaleFactors.z, 0,
		-posX * scaleFactors.x * cos(angle) + posY * scaleFactors.y * sin(angle), -posX * scaleFactors.y * sin(angle) - posY * scaleFactors.y * cos(angle), 0, 1
	);
}

void Camera::Follow(Entity* following)
{
	this->following = following;
}

void Camera::Unfollow()
{
	this->following = nullptr;
}

bool Camera::IsFollowing() const
{
	return this->following != nullptr;
}

void Camera::SetTransform(GraphicsDevice* gDevice)
{
	_gDevice = gDevice;
	Draw();
}

void Camera::Draw() 
{
	_gDevice->_device->SetTransform(D3DTS_PROJECTION, &orthographicMatrix);
	_gDevice->_device->SetTransform(D3DTS_WORLD, &identityMatrix);
	_gDevice->_device->SetTransform(D3DTS_VIEW, &viewMatrix);
}
