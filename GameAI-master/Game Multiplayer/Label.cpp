#include "Label.h"
#include "GameGlobal.h"

Label::Label(string s, int w, int h, D3DXVECTOR2 position, D3DCOLOR color)
{
	m_string = s;
	D3DXCreateFont(GameGlobal::Device
		, w, h
		, FW_NORMAL, 1
		, false, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE, (LPCWSTR) "Arial", &my_font);


	my_rect.left = (long)position.x;
	my_rect.top = (long)position.y;
	my_rect.bottom = my_rect.top + 500;
	my_rect.right = my_rect.left + 1000;

	my_color = color;
}

Label::~Label()
{
	//my_font->Release();
}

Label::Label()
{
}

void Label::Update(float dt)
{
}

void Label::Draw(string s, D3DCOLOR _color)
{
	if (my_font != NULL)
	{
		my_color = _color;
		if (s._Equal(""))
		{
			my_font->DrawTextA(GameGlobal::XSprite, m_string.c_str(), -1, &my_rect, DT_LEFT, my_color);
		}
		else
		{
			my_font->DrawTextA(GameGlobal::XSprite, s.c_str(), -1, &my_rect, DT_LEFT, my_color);
		}
	}
}

void Label::setPosition(D3DXVECTOR2 _pos)
{
	my_rect.left = (long)_pos.x;
	my_rect.top = (long)_pos.y;
	my_rect.bottom = my_rect.top + 500;
	my_rect.right = my_rect.left + 1000;
}
