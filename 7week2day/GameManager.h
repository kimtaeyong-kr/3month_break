#pragma once

enum BALL_DIR_X {
	eRIGHT,
	eLEFT,
};

enum BALL_DIR_Y {
	eDOWN,
	eUP,
};

#define BlockCount 35

class cGameManager
{
public:
	int m_Bar_X = 600;
	int m_Bar_Y = 680;

	int m_Ball_X = 600;
	int m_Ball_Y = 380;

	int KeySpeed = 5;

	bool keyLeftPressed = false;
	bool keyRightPressed = false;

private:
	BALL_DIR_X eDirX = eRIGHT;
	BALL_DIR_Y eDirY = eDOWN;

	//int BlockCount;
	cBlock BlockArray[BlockCount];

	cEffect m_Effect;
	cScore m_Score;
	cCrip m_Crip;

	void InitEffect(int x, int y);
	void InitCrip(int x, int y);

public:
	void BallMove();

	void CollisionBallAndBar();
	void CollisionBallAndBlock();

	void CreateBlock();
	void CreateScore();

	void DrawBlock(HWND hWnd, HDC bBufferDC);
	void DrawEffect(HWND hWnd, HDC hBufferDC);
	void DrawScore(HWND hWnd, HDC hBufferDC);
	void DrawCrip(HWND hWnd, HDC hBufferDC);

	void DestroyEffect();
	void DestroyScore();
	void DestroyCrip();

	void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
	void DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);

	//void DrawSprite(HWND hWnd, HDC hBufferDC, int x, int y,
	//			HBITMAP hBit, int index, int sprite_w, int sprite_h, COLORREF colorkey = RGB(0, 0, 0));
};
