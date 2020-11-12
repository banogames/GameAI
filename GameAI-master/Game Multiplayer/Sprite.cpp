#include "Sprite.h"
#include "GameGlobal.h"

Sprite::Sprite(const char * path, const D3DCOLOR & subColor)
{
	D3DXGetImageInfoFromFileA(path, &_imageInfo);

	// sprite rect
	_rect.left = 0;
	_rect.right = _imageInfo.Width;
	_rect.top = 0;
	_rect.bottom = _imageInfo.Height;

	D3DXCreateTextureFromFileExA(
		GameGlobal::Device,
		path,
		_imageInfo.Width,
		_imageInfo.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		subColor, // discard this color from Image
		&_imageInfo,
		NULL,
		&_texture);
}

Sprite::~Sprite()
{
	if (_texture != NULL)
		_texture->Release();
}

void Sprite::Draw(const D3DCOLOR & color)
{
	// create matrix from 2D info

	D3DXVECTOR2 scalingCenter = D3DXVECTOR2(_position.x, _position.y);
	D3DXVECTOR2 rotationCenter = scalingCenter;

	D3DXMatrixTransformation2D(
		&_matrix,
		&scalingCenter, // scaling center
		0, // scale rotation
		&_scale, // scale
		&rotationCenter, // rotation center
		_rotation, // rotation
		&_offset);

	D3DXMATRIX oldMatrix;

	// Draw from 3D info

	GameGlobal::XSprite->GetTransform(&oldMatrix);
	GameGlobal::XSprite->SetTransform(&_matrix);

	GameGlobal::XSprite->Draw(
		_texture,
		&_rect,
		&_center,
		&_position,
		color);

	GameGlobal::XSprite->SetTransform(&oldMatrix); // set oldMatrix => Xprite only apply for this class
}

void Sprite::setFipHorizontal(bool flipHor)
{
	if (_isFlipHorizontal != flipHor)
	{
		_isFlipHorizontal = flipHor;
		_scale.x = -_scale.x;
	}
}

void Sprite::setFlipVertical(bool flipVer)
{
	if (_isFlipVertical != flipVer)
	{
		_isFlipVertical = flipVer;
		_scale.y = -_scale.y;
	}
}

void Sprite::setRect(const RECT & rect)
{
	_rect = rect;
}

void Sprite::setOffset(const D3DXVECTOR2 & offset)
{
	_offset = D3DXVECTOR2((float)(int)offset.x, (float)(int)offset.y);
}

void Sprite::setPosition(const D3DXVECTOR2 & position)
{
	_position = D3DXVECTOR3((float)(int)position.x, (float)(int)position.y, 0.0f);
}

void Sprite::setCenter(const D3DXVECTOR2 & center)
{
	_center = D3DXVECTOR3((float)(int)center.x, (float)(int)center.y, 0.0f);
}

void Sprite::setScale(const D3DXVECTOR2 & scale)
{
	_scale = scale;
	if (_isFlipHorizontal)
		_scale.x = -_scale.x;
	if (_isFlipVertical)
		_scale.y = -_scale.y;
}
