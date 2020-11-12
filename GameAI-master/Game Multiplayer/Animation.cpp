#include "Animation.h"

void Animation::Update(float dt)
{
	// not have any frame info, return
	if (_frameInfos.size() <= 1)
	{
		return;
	}

	_count += dt;

	if (_count >= _time)
	{
		_frameIndex++;

		if (_frameIndex >= (int)_frameInfos.size())
		{
			_frameIndex = 0;
		}

		_count -= _time;
	}
}

void Animation::Draw(const D3DXVECTOR2 & position, const D3DXVECTOR2 & offset, bool isFlipHorizontal, bool isFlipVertical, const D3DCOLOR & color)
{
	FrameInfo currentFrameInfo = _frameInfos[_frameIndex];

	Sprite* sprite = currentFrameInfo.Sprite_;
	sprite->setRect(currentFrameInfo.Rect);
	sprite->setCenter(currentFrameInfo.Center);
	sprite->setScale(_scale);

	sprite->setPosition(position);
	sprite->setOffset(offset);
	sprite->setFipHorizontal(isFlipHorizontal);
	sprite->setFlipVertical(isFlipVertical);

	sprite->Draw(color);
}

void Animation::Reset()
{
	_frameIndex = 0;
	_count = 0.f;
}
