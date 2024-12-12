#pragma once

class cBlock
{
private:
	int m_x, m_y;
	bool m_bLive;

	HBITMAP m_hBitmap;

public:
	cBlock();
	~cBlock();

	RECT GetRect();
	void Init(int x, int y);
	void Draw(HWND hWnd, HDC hBufferDC);
	void SetLive(bool live) { m_bLive = live; }
	bool GetLive() { return m_bLive; }
};
