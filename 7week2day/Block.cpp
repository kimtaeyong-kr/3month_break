#include <Windows.h>

#include "Block.h"
#include "Score.h"

cBlock::cBlock() {
	m_hBitmap = (HBITMAP)LoadImage(NULL, L"block.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bLive = true;
}

void cBlock::Init(int x, int y) {
	m_x = x; m_y = y;
}

void cBlock::Draw(HWND hWnd, HDC hBufferDC) {
	HDC hDC = GetDC(hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, m_hBitmap);
	BITMAP bitmap;

	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

	BitBlt(hBufferDC, m_x, m_y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY);
	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);
}

RECT cBlock::GetRect() { // 위치 정보, 크기 - 충돌박스
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);
	RECT rc = { m_x, m_y, m_x + bitmap.bmWidth, m_y + bitmap.bmHeight };

	return rc;
}

//void DeleteBitmap()

cBlock::~cBlock() { //delete block
	DeleteObject(m_hBitmap);
}