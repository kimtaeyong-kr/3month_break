#include <Windows.h>
#include <gdiplus.h>

#include "Effect.h"
#include "Score.h"

using namespace Gdiplus;

#pragma comment(lib, "winmm.lib")

cEffect::cEffect() {
	m_nFrame = 0;
}

void cEffect::Init(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey) {
	if (image == NULL) {
		image = Gdiplus::Image::FromFile(L"flame_sprite2.png");
		Bitmap* pBitmap = (Bitmap*)image->Clone();
		pBitmap->GetHBITMAP(Color(0, 0, 0), &m_hBitmap);
	}

	m_x = x; m_y = y - 25;
	m_sprite_w = sprite_w;
	m_sprite_h = sprite_h;
	m_colorkey = colorkey;
	m_StartTime = timeGetTime();
	m_bLive = true;
}

void cEffect::Draw(HWND hWnd, HDC hBufferDC) {
	if (m_bLive == false) return;

	HDC hDC = GetDC(hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, m_hBitmap);
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

	TransparentBlt(hBufferDC, m_x, m_y, m_sprite_w, m_sprite_h, hTempDC, m_nFrame * m_sprite_w, 0, m_sprite_w, m_sprite_h, m_colorkey);
	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);

	if (m_StartTime + 30 < timeGetTime()) {
		m_nFrame++;

		if (m_nFrame > 5) {
			m_nFrame = 0;
			m_bLive = false;
		}
		m_StartTime = timeGetTime();
	}
}

void cEffect::Destroy() {
	DeleteObject(m_hBitmap);
}


cEffect::~cEffect() {
	//DeleteObject(m_hBitmap);
	//delete image;
}