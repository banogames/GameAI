#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Sprite
{
protected:
	D3DXIMAGE_INFO _imageInfo;
	LPDIRECT3DTEXTURE9 _texture;
	D3DXMATRIX _matrix; // use for Draw

	D3DXVECTOR3 _position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 _center = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // sprite center when you Draw it
	RECT _rect;
	D3DXVECTOR2 _scale = D3DXVECTOR2(1.0f, 1.0f);
	bool _isFlipHorizontal = false;
	bool _isFlipVertical = false;
	float _rotation = 0.0f;
	D3DXVECTOR2 _offset = D3DXVECTOR2(0.0f, 0.0f);

public:
	Sprite(const char* path, const D3DCOLOR &subColor = NULL);
	~Sprite();
	void Draw(const D3DCOLOR &color = D3DCOLOR_ARGB(255, 255, 255, 255));

	void setFipHorizontal(bool flipHor);
	void setFlipVertical(bool flipVer);
	void setRect(const RECT& rect);
	void setOffset(const D3DXVECTOR2& offset);
	void setPosition(const D3DXVECTOR2& position);
	void setCenter(const D3DXVECTOR2& center);
	void setScale(const D3DXVECTOR2& scale);
};
