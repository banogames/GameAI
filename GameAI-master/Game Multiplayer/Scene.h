#pragma once

#include "GameGlobal.h"
#include <vector>
#include <map>
#include "Label.h"
#include "GameDefine.h"

class Scene
{
protected:
	D3DCOLOR backgroundColor;
	std::map<int, bool> keyboard;
	bool isLeftMouseDown = false;
	bool isRightMouseDown = false;

public:
	Scene()
	{
		backgroundColor = D3DCOLOR_XRGB(0, 0, 0);
	}
	~Scene() {}

	virtual void Update(float _dt) {}
	virtual void Draw() {}

	virtual void OnKeyDown(int _keyCode)	
	{
		keyboard[_keyCode] = true; 
	}

	virtual void OnKeyUp(int _keyCode)
	{
		keyboard[_keyCode] = false;
	}

	virtual void OnLeftMouseDown(float x, float y)
	{
		isLeftMouseDown = true;
	}

	virtual void OnLeftMouseUp(float x, float y)
	{
		isLeftMouseDown = false;
	}

	virtual void OnRightMouseDown(float x, float y)
	{
		isRightMouseDown = true;
	}

	virtual void OnRightMouseUp(float x, float y)
	{
		isRightMouseDown = false;
	}

	D3DCOLOR GetBackcolor()
	{
		return backgroundColor;
	}
};

