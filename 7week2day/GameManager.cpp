#include <Windows.h>
#include <gdiplus.h>

#include "Block.h"
#include "Score.h"
#include "Effect.h"
#include "Crip.h"
#include "GameManager.h"

#pragma comment(lib, "winmm.lib")

int eSpeed_X = 0;
int eSpeed_Y = 3;

void cGameManager::CollisionBallAndBar() {
	POINT pt =				{ m_Ball_X + 20, m_Ball_Y + 39 };
	POINT pttop =			{ m_Ball_X + 20, m_Ball_Y };
	RECT rectleft =			{ m_Bar_X, m_Bar_Y, m_Bar_X + 57, m_Bar_Y + 28 };
	RECT rectleftpoint =    { m_Bar_X, m_Bar_Y, m_Bar_X, m_Bar_Y};
	RECT rect =  			{ m_Bar_X + 57, m_Bar_Y, m_Bar_X + 116, m_Bar_Y + 28 };
	RECT rectright =		{ m_Bar_X + 116, m_Bar_Y, m_Bar_X + 173, m_Bar_Y + 28 };
	RECT rectrightpoint =	{ m_Bar_X + 173, m_Bar_Y, m_Bar_X + 173, m_Bar_Y };
	RECT rectbottom =		{ m_Bar_X, m_Bar_Y, m_Bar_X + 173, m_Bar_Y + 28 };

	if ((PtInRect(&rectleft, pt)) && (eDirY == eDOWN)) {
		eDirY = eUP;

		if (eDirX == eRIGHT) {
			eSpeed_X = (eSpeed_X < 2) ? 2 : eSpeed_X - 1;
			eSpeed_Y = (eSpeed_Y < 3) ? 3 : eSpeed_Y - 1;
		}

		else if (eDirX == eLEFT) {
			eSpeed_X = (eSpeed_X > 5) ? 5 : eSpeed_X + 1;
			eSpeed_Y = (eSpeed_Y > 5) ? 5 : eSpeed_Y + 1;
		}
	}

	else if (PtInRect(&rectleftpoint, pt)) {
		if ((eDirY == eDOWN) && (eDirX == eRIGHT)) {
			eDirX = eLEFT;
			eDirY = eUP;
			eSpeed_X, eSpeed_Y = 5;
		}
	}

	else if ((PtInRect(&rect, pt)) && (eDirY == eDOWN)) {
		eDirY = eUP;
		eSpeed_X, eSpeed_Y = 3;
	}

	else if ((PtInRect(&rectright, pt)) && (eDirY == eDOWN)) {
		eDirY = eUP;

		if (eDirX == eRIGHT) {
			eSpeed_X = (eSpeed_X > 5) ? 5 : eSpeed_X + 1;
			eSpeed_Y = (eSpeed_Y > 5) ? 5 : eSpeed_Y + 1;
		}

		else if (eDirX == eLEFT) {
			eSpeed_X = (eSpeed_X < 2) ? 2 : eSpeed_X - 1;
			eSpeed_Y = (eSpeed_Y < 3) ? 3 : eSpeed_Y - 1;
		}
	}

	else if (PtInRect(&rectrightpoint, pt)) {
		if ((eDirY == eDOWN) && (eDirX == eLEFT)) {
			eDirX = eRIGHT;
			eDirY = eUP;
			eSpeed_X, eSpeed_Y = 5;
		}
	}
	
	else if ((PtInRect(&rectbottom, pttop)) && (eDirY == eUP)) {
		eDirY = eDOWN;
	}

	if ((eDirY == eDOWN))
		m_Ball_Y += eSpeed_Y;
	else if (eDirY == eUP)
		m_Ball_Y -= eSpeed_Y;
}

void cGameManager::CollisionBallAndBlock() {
	//ball의 위쪽 점
	POINT pt = { m_Ball_X + 20, m_Ball_Y };
	POINT pt2 = { m_Ball_X + 20, m_Ball_Y + 39 };
	POINT pt3 = { m_Ball_X + 20, m_Ball_Y + 20 };

	for (int i = 0; i < BlockCount; ++i) {
		if (!BlockArray[i].GetLive()) continue;

		RECT rect = BlockArray[i].GetRect();
		RECT crect = m_Crip.cGetRect();

		if (PtInRect(&rect, pt) || PtInRect(&rect, pt2)) {
			eDirY = (eDirY == eUP ? eDOWN : eUP);

			BlockArray[i].SetLive(false);
			m_Crip.SetLive(true);

			InitEffect(rect.left, rect.top);
			InitCrip(rect.left + 90, rect.top + 5);

			m_Score.AddScore();

			break;
		}

		if (PtInRect(&crect, pt3)) {
			m_Crip.SetLive(false);

			m_Score.AddScore();

			break;
		}
	}
}

void cGameManager::BallMove() {
	cGameManager g_GameMng;

	if (eDirX == eRIGHT) {
		m_Ball_X += eSpeed_X;
		m_Ball_X >= 985 ? eDirX = eLEFT : eDirX = eRIGHT;
	}
	else if (eDirX == eLEFT) {
		m_Ball_X -= eSpeed_X;
		m_Ball_X <= 0 ? eDirX = eRIGHT : eDirX = eLEFT;
	}

	if (eDirY == eDOWN) {
		m_Ball_Y += eSpeed_Y;

		if ((m_Ball_Y >= 729)) {
			m_Ball_Y = 729;
			eDirY = eUP;

			m_Score.DeleteScore();

			if (m_Score.GetScore() <= 0) {
				//m_Score.SetScore(0);
				eSpeed_X = 0;
				eSpeed_Y = 0;
				MessageBoxW(NULL, L"게임 종료", L"game over", 0);
				PostQuitMessage(0);
			}
		}
		else
			eDirY = eDOWN;
	}
	else if (eDirY == eUP) {
		m_Ball_Y -= eSpeed_Y;
		m_Ball_Y <= 0 ? eDirY = eDOWN : eDirY = eUP;
	}

}

void cGameManager::CreateBlock() {
	for (int i = 0; i < BlockCount; ++i) {
		BlockArray[i].Init(10 + (i % 7) * 130 + 15 * (i % 7), 80 + (32 * (i / 7)));
	}
}

void cGameManager::DrawBlock(HWND hWnd, HDC hBufferDC) {
	for (int i = 0; i < BlockCount; ++i) {
		if (BlockArray[i].GetLive() == false) continue;

		BlockArray[i].Draw(hWnd, hBufferDC);
	}
}

void cGameManager::InitEffect(int x, int y) {
	m_Effect.Init(x, y, 100, 100, RGB(0, 0, 0));
}

void cGameManager::DrawEffect(HWND hWnd, HDC hBufferDC) {
	m_Effect.Draw(hWnd, hBufferDC);
}

void cGameManager::DestroyEffect() {
	m_Effect.Destroy();
}

void cGameManager::CreateScore() {
	m_Score.Create(10, 10, 72, 77, RGB(0, 0, 0));
}

void cGameManager::DrawScore(HWND hWnd, HDC hBufferDC) {
	m_Score.Draw(hWnd, hBufferDC);
}

void cGameManager::DestroyScore() {
	m_Crip.SetLive(false);
}

void cGameManager::InitCrip(int x, int y) {
	m_Crip.InitCrip(x, y, 34, 31, RGB(255, 255, 255));
}

void cGameManager::DrawCrip(HWND hWnd, HDC hBufferDC) {
	if (m_Crip.GetLive() == true) {
		m_Crip.DrawCrip(hWnd, hBufferDC);
	}
}

void cGameManager::DestroyCrip() {
	m_Crip.DestroyCrip();
}

void cGameManager::DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit) {
	HDC hDC = GetDC(hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);
	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	BitBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY);
	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);
}

void cGameManager::DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit) {
	HDC hDC = GetDC(hWnd);
	HDC hTempDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);
	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));
	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
	ReleaseDC(hWnd, hDC);
}