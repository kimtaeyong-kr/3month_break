#pragma once
#include <gdiplus.h>

using namespace Gdiplus;

class cScore {
private:
	int m_x, m_y;
	int m_sprite_w, m_sprite_h;
	int m_CurrentScore;	// 현재 점수

	COLORREF m_colorkey;
	HBITMAP m_hBitmap;

	Gdiplus::Image* image = NULL;

public:
	cScore();
	~cScore();

	int cScore::GetScore() {
		return m_CurrentScore;
	}

	void cScore::SetScore(int score) {
		m_CurrentScore = score;
	}

	void Create(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey);
	void Draw(HWND hWnd, HDC hBufferDC);
	void AddScore() { m_CurrentScore++; };
	void DeleteScore() { m_CurrentScore--; };
	void DestroyScore();
};