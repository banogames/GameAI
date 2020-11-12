#pragma once

#include <string>
#include <d3dx9.h>
using namespace std;

class Label
{
public:
	Label(string s, int w, int h, D3DXVECTOR2 position = D3DXVECTOR2(0.f, 0.f), D3DCOLOR color = D3DCOLOR_XRGB(255, 255, 255));
	~Label();
	Label();
	ID3DXFont				*my_font = NULL; //Font dung de ve chu
	RECT my_rect; //RECT chua myFont
	std::string m_string; //Support Font
	D3DCOLOR my_color;
	void Update(float dt);
	void Draw(string s = "", D3DCOLOR _color = D3DCOLOR_XRGB(255, 255, 255));
	void setPosition(D3DXVECTOR2 _pos);
};

