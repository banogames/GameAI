#pragma once

#include "Sprite.h"
#include <vector>

class FrameInfo
{
public:
	Sprite* Sprite_;
	RECT Rect;
	D3DXVECTOR2 Center;

public:
	FrameInfo(Sprite* sprite, int left, int right, int top, int bottom, const D3DXVECTOR2& center)
	{
		Sprite_ = sprite;
		Rect.left = left;
		Rect.right = right;
		Rect.top = top;
		Rect.bottom = bottom;
		Center = center;
	}
};

class Animation
{
	std::vector<FrameInfo> _frameInfos; // thông tin về các frame
	int _frameIndex = 0; // frame hiện tại
	float _time = 0.f; // thời gian chuyển frame
	float _count = 0.f; // đếm => chuyển frame
	D3DXVECTOR2 _scale = D3DXVECTOR2(1.f, 1.f);

public:
	// khởi tạo animation với: thời gian chuyển frame
	Animation(float time = 0.f) : _time(time) {}
	~Animation() {}

	// thêm vào animation các frame
	void addFrameInfo(const FrameInfo& frameInfo)
	{
		_frameInfos.push_back(frameInfo);
	}

	void Update(float dt);
	void Draw(const D3DXVECTOR2& position, const D3DXVECTOR2& offset = D3DXVECTOR2(0.0f, 0.0f), bool isFlipHorizontal = false,
		bool isFlipVertical = false, const D3DCOLOR& color = D3DCOLOR_ARGB(255, 255, 255, 255));

	void Reset();
	void setScale(D3DXVECTOR2 scale) { _scale = scale; }
};

