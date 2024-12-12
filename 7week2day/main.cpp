#include <windows.h>
#include <gdiplus.h>

#include "Block.h"
#include "Effect.h"
#include "Score.h"
#include "Crip.h"
#include "GameManager.h"

#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

cGameManager g_GameMng;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //콜백 함수

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wcex = { 0, }; //모든 값을 0으로 초기화

	wcex.style = CS_HREDRAW | CS_VREDRAW; //비트 연산자
	wcex.lpfnWndProc = WndProc; //프로시저 -> 콜백함수
	wcex.hInstance = hInstance; //제어권
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); //윈도우 색 지정
	wcex.lpszClassName = L"winclass"; //클래스 이름

	RegisterClass(&wcex); //윈도우 형태를 등록
						  //윈도우를 만든다.
	HWND hWnd = CreateWindow(L"winclass", L"벽돌깨기", WS_OVERLAPPEDWINDOW, 500, 100, 1024, 768, nullptr, nullptr, hInstance, nullptr);

	//윈도우를 보여준다.
	ShowWindow(hWnd, SW_SHOW);

	//윈도우를 갱신한다.
	UpdateWindow(hWnd);

	MSG msg;

	//GetMessage 내부에서 WM_QUIT가 나오면 false를 리턴하고 while문을 빠져 나온다.
	while (GetMessage(&msg, nullptr, 0, 0) == true) //큐 구조 -> 비어 있으면 무한정 기다린다.
	{
		TranslateMessage(&msg); //메세지 번역
		DispatchMessage(&msg); //메세지 처리 ->WndProc호출
	}

	// SendMessage -> 메시지 큐를 거치지 않고 윈도우 프로시저를 직접 호출
	// PostMessage -> 메시지 큐에 메시지만 넣는다.

	return (int)msg.wParam;
}

HBITMAP hBackBitmap;
HBITMAP hBarBitmap;
HBITMAP hBallBitmap;
HBITMAP hBitmapEffect;
HBITMAP hBitMapBuffer;
HANDLE hTimer;
RECT crt;

#define TIMERID 1

bool keyLeftPressed = false;
bool keyRightPressed = false;
ULONG_PTR gdiplusToken;
GdiplusStartupInput gdiplusStartupInput;

int sprite_index = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	HDC hDC;
	HDC hMemDC;
	PAINTSTRUCT ps;
	BITMAP bit;
	cScore m_Score;

	switch (message) {
	case WM_CREATE: {
		RECT window_rc;
		GetWindowRect(hWnd, &window_rc);

		RECT crt_rc;
		GetClientRect(hWnd, &crt);

		int delta_X = (window_rc.right - window_rc.left) - crt.right;
		int delta_Y = (window_rc.bottom - window_rc.top) - crt.bottom;

		SetWindowPos(hWnd, 0, 0, 0, 1024 + delta_X, 768 + delta_Y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); //gdiplus 초기화

		g_GameMng.CreateBlock();
		g_GameMng.CreateScore();

		//image = Gdiplus::Image::FromFile(L"flame_sprite2.png");
		//Bitmap* pBitmap = (Bitmap*)image->Clone();
		//pBitmap->GetHBITMAP(Color(0, 0, 0), &hBitmapEffect);

		hTimer = (HANDLE)SetTimer(hWnd, TIMERID, 1, NULL);
	}
	break;

	case WM_TIMER: {
		RECT window_rc;
		GetWindowRect(hWnd, &window_rc);

		RECT crt_rc;
		GetClientRect(hWnd, &crt);

		if (keyLeftPressed) {
			if (g_GameMng.m_Bar_X > 0) {
				g_GameMng.m_Bar_X -= g_GameMng.KeySpeed;
			}
		}

		if (keyRightPressed) {
			if (g_GameMng.m_Bar_X < 851) {
				g_GameMng.m_Bar_X += g_GameMng.KeySpeed;
			}
		}

		g_GameMng.BallMove();
		g_GameMng.CollisionBallAndBar();
		g_GameMng.CollisionBallAndBlock();
		HDC hDC = GetDC(hWnd);

		if (hBitMapBuffer == NULL) {
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); //가상 비트맵 파일(화면 크기)
		}

		HDC hBufferDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBuf = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer); //OnPaint에서 사용할 버퍼 선택

		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, g_GameMng.m_Bar_X, 680, hBarBitmap);
		g_GameMng.DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_GameMng.m_Ball_X, g_GameMng.m_Ball_Y, hBallBitmap);
		g_GameMng.DrawBlock(hWnd, hBufferDC);
		g_GameMng.DrawEffect(hWnd, hBufferDC);
		g_GameMng.DrawScore(hWnd, hBufferDC);
		g_GameMng.DrawCrip(hWnd, hBufferDC);

		//g_GameMng.DrawSprite(hWnd, hBufferDC, 0, 50, hBitmapEffect, sprite_index, 100, 100);
		//sprite_index++;
		//if (sprite_index > 5) sprite_index = 0;

		SelectObject(hBufferDC, hOldBuf);
		DeleteDC(hBufferDC);
		ReleaseDC(hWnd, hDC);
		InvalidateRect(hWnd, NULL, true);
	}
	break;

	case WM_KEYDOWN: {
		switch (wParam) {
		case VK_LEFT: {
			keyLeftPressed = true;
			break;
		}
		case VK_RIGHT: {
			keyRightPressed = true;
			break;
		}
		default: break;
		}
	}
	break;

	case WM_KEYUP: {
		switch (wParam) {
		case VK_LEFT: {
			keyLeftPressed = false;
			break;
		}
		case VK_RIGHT: {
			keyRightPressed = false;
			break;
		}
		default: break;
		}
	}
	break;

	case WM_PAINT: {
		hDC = BeginPaint(hWnd, &ps); {
			HBITMAP hOldBit;

			hMemDC = CreateCompatibleDC(hDC);
			hOldBit = (HBITMAP)SelectObject(hMemDC, hBitMapBuffer);

			BitBlt(hDC, 0, 0, crt.right, crt.bottom, hMemDC, 0, 0, SRCCOPY);
			SelectObject(hMemDC, hOldBit);
			DeleteDC(hMemDC);
		}
		EndPaint(hWnd, &ps);
	}
				 break;

	case WM_ERASEBKGND:
		return TRUE;

	case WM_DESTROY: { //이벤트
		DeleteObject(hBackBitmap);
		DeleteObject(hBarBitmap);
		DeleteObject(hBallBitmap);
		DeleteObject(hBitMapBuffer);
		GdiplusShutdown(gdiplusToken);

		g_GameMng.DestroyEffect();
		g_GameMng.DestroyScore();
		g_GameMng.DestroyCrip();

		KillTimer(hWnd, TIMERID);
		PostQuitMessage(0); //-> 메시지 큐에 WM_QUIT가 들어간다.
	}
				   break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}