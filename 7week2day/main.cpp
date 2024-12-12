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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //�ݹ� �Լ�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wcex = { 0, }; //��� ���� 0���� �ʱ�ȭ

	wcex.style = CS_HREDRAW | CS_VREDRAW; //��Ʈ ������
	wcex.lpfnWndProc = WndProc; //���ν��� -> �ݹ��Լ�
	wcex.hInstance = hInstance; //�����
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); //������ �� ����
	wcex.lpszClassName = L"winclass"; //Ŭ���� �̸�

	RegisterClass(&wcex); //������ ���¸� ���
						  //�����츦 �����.
	HWND hWnd = CreateWindow(L"winclass", L"��������", WS_OVERLAPPEDWINDOW, 500, 100, 1024, 768, nullptr, nullptr, hInstance, nullptr);

	//�����츦 �����ش�.
	ShowWindow(hWnd, SW_SHOW);

	//�����츦 �����Ѵ�.
	UpdateWindow(hWnd);

	MSG msg;

	//GetMessage ���ο��� WM_QUIT�� ������ false�� �����ϰ� while���� ���� ���´�.
	while (GetMessage(&msg, nullptr, 0, 0) == true) //ť ���� -> ��� ������ ������ ��ٸ���.
	{
		TranslateMessage(&msg); //�޼��� ����
		DispatchMessage(&msg); //�޼��� ó�� ->WndProcȣ��
	}

	// SendMessage -> �޽��� ť�� ��ġ�� �ʰ� ������ ���ν����� ���� ȣ��
	// PostMessage -> �޽��� ť�� �޽����� �ִ´�.

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

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); //gdiplus �ʱ�ȭ

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
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); //���� ��Ʈ�� ����(ȭ�� ũ��)
		}

		HDC hBufferDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBuf = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer); //OnPaint���� ����� ���� ����

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

	case WM_DESTROY: { //�̺�Ʈ
		DeleteObject(hBackBitmap);
		DeleteObject(hBarBitmap);
		DeleteObject(hBallBitmap);
		DeleteObject(hBitMapBuffer);
		GdiplusShutdown(gdiplusToken);

		g_GameMng.DestroyEffect();
		g_GameMng.DestroyScore();
		g_GameMng.DestroyCrip();

		KillTimer(hWnd, TIMERID);
		PostQuitMessage(0); //-> �޽��� ť�� WM_QUIT�� ����.
	}
				   break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}