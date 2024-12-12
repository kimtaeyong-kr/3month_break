#pragma once

class cCrip {
private:
	int m_x, m_y;
	int m_sprite_w, m_sprite_h;
	int m_nFrame;
	bool m_bLive;

	HBITMAP m_hBitmap;
	COLORREF m_colorkey;

	Gdiplus::Image* image = NULL;

public:
	cCrip();
	~cCrip();

	void InitCrip(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey);
	void DrawCrip(HWND hWnd, HDC hBufferDC);
	RECT cGetRect();
	void SetLive(bool live) { m_bLive = live; }
	bool GetLive() { return m_bLive; }
	void DestroyCrip();
};