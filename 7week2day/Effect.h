#pragma once

class cEffect{
private:
	int m_x, m_y;
	int m_sprite_w, m_sprite_h;
	int m_nFrame;
	bool m_bLive;

	HBITMAP m_hBitmap;
	COLORREF m_colorkey;
	DWORD m_StartTime;

	Gdiplus::Image* image = NULL;
public:
	cEffect();
	~cEffect();

	void Init(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey);
	void Draw(HWND hWnd, HDC hBufferDC);
	void Destroy();
};
