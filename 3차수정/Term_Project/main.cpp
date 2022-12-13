#pragma once
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include "stdafx.h"
#include "Player.h"
#include "Monster.h"
#include "Interaction_Object.h"
#include "NetworkFunc.h"

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;

//
//// 서버 연결 변수
//#define SERVERPORT 9000
//#define SERVERIP "127.0.0.1"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hWnd = CreateWindow
	(lpszClass, lpszWindowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, Window_Size_X, Window_Size_Y,
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	InitClient();
	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

void LoadImage();

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hdc, memdc1, memdc2;
	HBITMAP hBitmap1, hBitmap2;
	HBRUSH hBrush, oldBrush;

	// BackGround for Scroll 
	static int scroll_x;

	static int dest_x = 10300;
	static int dest_y = 600;
	static int is_game_fail = false;
	static int is_game_success = false;

	static Interaction_Object rever[3];
	static Interaction_Object button[12];
	static Interaction_Object portal;

	switch (uMsg) {

	case WM_CREATE:
		LoadImage();
		SetTimer(hWnd, 1, 34, NULL);

		for (int i = 0; i < RAW; ++i) {
			for (int j = 0; j < COLUMN; ++j) {
				Board[i][j].left = 0 + j * BLOCK_SIZE;
				Board[i][j].right = 50 + j * BLOCK_SIZE;
				Board[i][j].top = 0 + i * BLOCK_SIZE;
				Board[i][j].bottom = 50 + i * BLOCK_SIZE;
			}
		}

		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		InputKey();
		break;
	case WM_TIMER:
		for (auto& player : players)
			player.UpdateAnimation();

		for (auto& monster : monsters)
			monster.second.UpdateAnimation();

		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
		if (ID == -1) break;

		hdc = BeginPaint(hWnd, &ps);

		memdc1 = CreateCompatibleDC(hdc);
		hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);
		SelectObject(memdc1, hBitmap1);

		// 1. 배경 그리기
		{
			auto& player = players[ID];
			if (player.x >= 200) {
				Manager::GetInstance().bg.Draw(memdc1, -Window_Size_X + scroll_x + player.x - 200, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
				Manager::GetInstance().bg.Draw(memdc1, scroll_x + player.x - 200, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
			}
			else {
				Manager::GetInstance().bg.Draw(memdc1, -Window_Size_X + scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
				Manager::GetInstance().bg.Draw(memdc1, scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
			}

			// 2. 발판 그리기
			for (int i = 0; i < RAW; ++i) {
				for (int j = 0; j < COLUMN; ++j) {
					if (Board[i][j].right > player.x - 500 && Board[i][j].left < player.x + 800)
					{
						if (Map[i][j] == WALL)
						{
							Manager::GetInstance().wall.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 25);
						}

						if (Map[i][j] == MAGMA)
						{
							Manager::GetInstance().magma.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 50);
						}
					}
				}
			}

			for (int i = 0; i < player.life; ++i) {
				if (player.x > 200) {
					Manager::GetInstance().My_heart.Draw(memdc1, 30 + 45 * i + player.x - 200, 20, 45, 46, 0, 0, 45, 46);
				}
				else {
					Manager::GetInstance().My_heart.Draw(memdc1, 30 + 45 * i + 0, 20, 45, 46, 0, 0, 45, 46);
				}
			}
		}

		for (auto& player : players)
		{
			player.getDrawImageByState().Draw(memdc1, player.x, player.y, 32, 32, player.anim, player.dir == -1 ? 32 : 1, 32, 32);
		}

		for (auto element : monsters)
		{
			auto& monster = element.second;
			if (monster.life > 0)
			{
				Manager::GetInstance().monster.Draw(memdc1, monster.x, monster.y, 32, 32, monster.anim, 0, 32, 32);
			}
		}

		for (auto& bullet : bullets) {
			if (bullet.second.isAttack) {
				Manager::GetInstance().bullet.Draw(memdc1, bullet.second.x, bullet.second.y, 32, 16);
			}
		}

		for (auto& obstacle : obstacles)
		{
			if(obstacle.second.isActive)
				Manager::GetInstance().garo.Draw(memdc1, obstacle.second.x, obstacle.second.y, 32, 16);
		}
		// 9. Heart 그리기
		// 10. 최종 Portal 그리기

		for (auto& obj : interactionObjects)
		{
			auto& interactionObject = obj.second;
			switch (interactionObject.type)
			{
			case INTER_OBJ::BUTTON:
				interactionObject.getDrawImageByState().Draw(memdc1, interactionObject.x, interactionObject.y);
				break;
			case INTER_OBJ::DOOR:
				interactionObject.getDrawImageByState().Draw(memdc1, interactionObject.x, interactionObject.y, 50, 50, 0, interactionObject.state == true ? 100 : 0, 50, 50);
				break;
			case INTER_OBJ::REVER:
				interactionObject.getDrawImageByState().Draw(memdc1, interactionObject.x, interactionObject.y);
				break;
			case INTER_OBJ::PORTAL:
				interactionObject.getDrawImageByState().Draw(memdc1, interactionObject.x, interactionObject.y, 52, 52, 0, 0, 52, 52);
				break;
			}
		}

		POINT point;
		point.x = 0;
		point.y = 0;

		if (countDown != -1)
		{
			switch (countDown)
			{
			case 5:
				Manager::GetInstance().Number.Draw(memdc1, Window_Size_X / 2 - 50, Window_Size_Y / 2 - 50, 103, 103, 0, 103, 103, 103);
				break;
			case 4:
				Manager::GetInstance().Number.Draw(memdc1, Window_Size_X / 2 - 50, Window_Size_Y / 2 - 50, 103, 103, 412, 0, 103, 103);
				break;
			case 3:
				Manager::GetInstance().Number.Draw(memdc1, Window_Size_X / 2 - 50, Window_Size_Y / 2 - 50, 103, 103, 309, 0, 103, 103);
				break;
			case 2:
				Manager::GetInstance().Number.Draw(memdc1, Window_Size_X / 2 - 50, Window_Size_Y / 2 - 50, 103, 103, 206, 0, 103, 103);
				break;
			case 1:
				Manager::GetInstance().Number.Draw(memdc1, Window_Size_X / 2 - 50, Window_Size_Y / 2 - 50, 103, 103, 103, 0, 103, 103);
				break;
			default:
				break;
			}
		}

		if (isClearGame)
		{
			Manager::GetInstance().SuccessGame.Draw(memdc1, players[ID].x - 200, 0);
		}

		if (players[ID].x > 200) {
			BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, players[ID].x - 200, 0, SRCCOPY);
		}
		else {
			BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);
		}

		DeleteObject(SelectObject(memdc1, hBitmap1));
		DeleteDC(memdc1);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void LoadImage()
{
	Manager::GetInstance().bg.Load(L"background.png");
	bg_width = Manager::GetInstance().bg.GetWidth();
	bg_height = Manager::GetInstance().bg.GetHeight();

	Manager::GetInstance().wall.Load(L"block_wall.png");
	Manager::GetInstance().Number.Load(L"CountDown.png");
	Manager::GetInstance().door.Load(L"Door.png");

	Manager::GetInstance().monster.Load(L"pacman.png");
	Manager::GetInstance().monster_idle.Load(L"pacman.png");
	Manager::GetInstance().monster_dead.Load(L"monster_dead.png");

	Manager::GetInstance().frog_idle.Load(L"frog_idle.png");
	Manager::GetInstance().frog_move.Load(L"frog_move.png");
	Manager::GetInstance().frog_jump.Load(L"frog_jump.png");
	Manager::GetInstance().frog_fall.Load(L"frog_fall.png");
	Manager::GetInstance().frog_attack.Load(L"frog_attack.png");

	Manager::GetInstance().bullet.Load(L"bullet.png");

	Manager::GetInstance().effect.Load(L"effect.png");

	Manager::GetInstance().magma.Load(L"magma.jpg");

	Manager::GetInstance().garo.Load(L"garo.png");
	Manager::GetInstance().sero.Load(L"sero.png");

	Manager::GetInstance().My_heart.Load(L"heart.png");

	Manager::GetInstance().portal_img.Load(L"portal.png");

	Manager::GetInstance().FailGame.Load(L"FailGame.png");
	Manager::GetInstance().SuccessGame.Load(L"SuccessGame.png");

	Manager::GetInstance().rever_nomal.Load(L"Rever1.png");
	Manager::GetInstance().rever_pull.Load(L"Rever2.png");

	Manager::GetInstance().button_normal.Load(L"Button1.png");
	Manager::GetInstance().button_pull.Load(L"Button2.png");
}