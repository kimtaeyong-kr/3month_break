#include <Windows.h>
#include <gdiplus.h>

#include "Crip.h"

using namespace Gdiplus;

#pragma comment(lib, "winmm.lib")

cCrip::cCrip() {
	m_nFrame = 0;
}

void cCrip::InitCrip(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey) {
	if (image == NULL) {
		image = Gdiplus::Image::FromFile(L"Crip.bmp");
		Bitmap* pBitmap = (Bitmap*)image->Clone();
		pBitmap->GetHBITMAP(Color(0, 0, 0), &m_hBitmap);
	}

	m_x = x; m_y = y;
	m_sprite_w = sprite_w;
	m_sprite_h = sprite_h;
	m_colorkey = colorkey;
	m_bLive = true;
}

void cCrip::DrawCrip(HWND hWnd, HDC hBufferDC) {
	HDC hDC = GetDC(hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, m_hBitmap);
	BITMAP bitmap;

	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

	TransparentBlt(hBufferDC, m_x, m_y, m_sprite_w, m_sprite_h, hTempDC, m_nFrame * m_sprite_w, 0, m_sprite_w, m_sprite_h, m_colorkey);
	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);
}

RECT cCrip::cGetRect() {
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);
	RECT rc = { m_x, m_y, m_x + bitmap.bmWidth, m_y + bitmap.bmHeight };

	return rc;
}

void cCrip::DestroyCrip() {
	DeleteObject(m_hBitmap);
}


cCrip::~cCrip() {
	DeleteObject(m_hBitmap);
}