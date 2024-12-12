#include <Windows.h>
#include <gdiplus.h>

#include "Effect.h"
#include "Score.h"

using namespace Gdiplus;

#pragma comment(lib, "winmm.lib")

cScore::cScore() {
	m_CurrentScore = 0;
}

void cScore::Create(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey){
	if (image == NULL){
		image = Gdiplus::Image::FromFile(L"numbers.png");
		Bitmap* pBitmap = (Bitmap*)image->Clone();
		pBitmap->GetHBITMAP(Color(0, 0, 0), &m_hBitmap);
	}

	m_x = x; m_y = y;
	m_sprite_w = sprite_w;
	m_sprite_h = sprite_h;
	m_colorkey = colorkey;
}

int Digits(int Score) {	// 9 -> 1 , 10 -> 2
	int pos = 1, i;
	if (Score < 0) Score *= (-1);
	for (i = 0; ; i++, pos++) {
		if ((Score /= 10) <= 0) {
			break;
		}
	}
	return pos;
}

void cScore::Draw(HWND hWnd, HDC hBufferDC){
	int NumberPos = Digits(m_CurrentScore);
	HDC hDC = GetDC(hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, m_hBitmap);	// 파일에서 가져오는 부분
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

	int DigitsNumber = m_CurrentScore;

	// 숫자를 찍는 부분
	for (int i = 0; i < NumberPos; i++) {
		// 자릿수별 숫자
		int tempNum;

		if (i == 0) {
			tempNum = DigitsNumber % 10; 
			TransparentBlt(
				hBufferDC,
				m_x + 80,
				m_y,
				m_sprite_w,
				m_sprite_h,
				hTempDC,
				m_sprite_w * tempNum,
				0,
				m_sprite_w,
				m_sprite_h,
				m_colorkey
			);
		}

		else {
			DigitsNumber /= 10;
			tempNum = DigitsNumber % 10;

			TransparentBlt(
				hBufferDC,
				m_x,
				m_y,
				m_sprite_w,
				m_sprite_h,
				hTempDC,
				m_sprite_w * tempNum,
				0,
				m_sprite_w,
				m_sprite_h,
				m_colorkey
			);
		}

	}

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);
}

void cScore::DestroyScore(){
	DeleteObject(m_hBitmap);
}


cScore::~cScore(){
	//DeleteObject(m_hBitmap);
	//delete image;
}