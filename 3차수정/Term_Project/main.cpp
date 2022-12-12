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
bool CollisionHelper(RECT, RECT);
void GaroObstacle();
void SeroObstacle();
void InitMonster(int m_x[MONSTER_AMOUNT], int m_y[MONSTER_AMOUNT]);


struct Potal
{

};

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    static HDC hdc, memdc1, memdc2;
    HBITMAP hBitmap1, hBitmap2;
    HBRUSH hBrush, oldBrush;


    //static RECT p_rect;
    static RECT w_rect[820];

    static int w_rect_count = 0;

    static RECT obs_g_rect[OBS_GARO_COUNT];
    static RECT obs_s_rect[OBS_SERO_COUNT];

    // BackGround for Scroll 
    static int scroll_x;

    // 물리

    static float  PLAYER_ACC = 1.f;
    static float  PLAYER_FRICTION = -0.2f;
    static float  PLAYER_GRAVITY = 0.9f;
    
    static int dest_x = 10300;
    static int dest_y = 600;
    static int is_game_fail = false;
    static int is_game_success = false;

    static Interaction_Object rever[3];
    static Interaction_Object button[12];
    static Interaction_Object portal;


    //static Player player(300, 420, Manager::GetInstance().frog_idle);


    /*static Monster monster[MONSTER_AMOUNT]{
    
        Monster(630, 670, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE)
      , Monster(700, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE), 
        Monster(400, 165, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE)
      , Monster(970, 420, Manager::GetInstance().monster_idle,  MONSTER_TYPE::MONSTER_MOVE), 
        Monster(1000, 670, Manager::GetInstance().monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(2300, 670, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE), 
        Monster(2400, 420, Manager::GetInstance().monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(2400, 165, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE), 
        Monster(2800, 165, Manager::GetInstance().monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(4300, 670, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE), 
        Monster(4500, 165, Manager::GetInstance().monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(4750, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE), 
        Monster(5400, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(5550, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(5850, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(6150, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(6900, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7050, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(7050, 165, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7050, 670, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(7500, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7500, 170, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(7700, 670, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7800, 625, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(7850, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8100, 170, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(8100, 625, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8300, 420, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(8700, 370, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8800, 270, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(8800, 720, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8950, 320, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE), 
        Monster(8900, 165, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_PEACE)
    };*/

    switch (uMsg) {

    case WM_CREATE:
        LoadImage();
        SetTimer(hWnd, 1, 16, NULL);
      //  //레버 초기화
      //  rever[0].x = 9525;
      //  rever[0].y = 700;
      //  rever[0].draw_image = Manager::GetInstance().rever_nomal;

      //  rever[1].x = 9525;
      //  rever[1].y = 400;
      //  rever[1].draw_image = Manager::GetInstance().rever_nomal;

      //  rever[2].x = 9525;
      //  rever[2].y = 150;
      //  rever[2].draw_image = Manager::GetInstance().rever_nomal;

      //  rever[0].SetCollsionBox();
      //  rever[1].SetCollsionBox();
      //  rever[2].SetCollsionBox();


      //  rever[0].type = INTER_OBJ::REVER;
      //  rever[1].type = INTER_OBJ::REVER;
      //  rever[2].type = INTER_OBJ::REVER;



      //  //레버 초기화
      //  button[0].x = 2420;
      //  button[0].y = 670;
      //  button[0].draw_image = Manager::GetInstance().button_normal;

      //  button[1].x = 2420;
      //  button[1].y = 420;
      //  button[1].draw_image = Manager::GetInstance().button_normal;

      //  button[2].x = 2420;
      //  button[2].y = 170;
      //  button[2].draw_image = Manager::GetInstance().button_normal;

      // button[3].x = 8425;
      // button[3].y = 220;
      // button[3].draw_image = Manager::GetInstance().button_normal;
      // 
      // button[4].x = 8425;
      // button[4].y = 420;
      // button[4].draw_image = Manager::GetInstance().button_normal;

      // button[5].x = 8425;
      // button[5].y = 670;
      // button[5].draw_image = Manager::GetInstance().button_normal;


      // button[0].SetCollsionBox();
      // button[1].SetCollsionBox();
      // button[2].SetCollsionBox();
      // button[3].SetCollsionBox();
      // button[4].SetCollsionBox();
      // button[5].SetCollsionBox();

      // button[0].type = INTER_OBJ::BUTTON;
      // button[1].type = INTER_OBJ::BUTTON;
      // button[2].type = INTER_OBJ::BUTTON;
      // button[3].type = INTER_OBJ::BUTTON;
      // button[4].type = INTER_OBJ::BUTTON;
      // button[5].type = INTER_OBJ::BUTTON;

      // portal.x = 9950;
      // portal.y = 390;
      // portal.draw_image = Manager::GetInstance().button_normal;
      // portal.SetCollsionBox();
      // portal.type = INTER_OBJ::PORTAL;

      //player.ChangeState(STATE::IDLE);

      //  for (int i = 0; i < MONSTER_AMOUNT; ++i) {
      //      monsters[i].ChangeState(STATE::IDLE);
      //  }


      //  for (int i = 0; i < RAW; ++i) {
      //      for (int j = 0; j < COLUMN; ++j) {
      //          Board[i][j].left = 0 + j * BLOCK_SIZE;
      //          Board[i][j].right = 50 + j * BLOCK_SIZE;
      //          Board[i][j].top = 0 + i * BLOCK_SIZE;
      //          Board[i][j].bottom = 50 + i * BLOCK_SIZE;
      //      }
      //  }

      //  for (int i = 0; i < RAW; ++i) {
      //      for (int j = 0; j < COLUMN; ++j) {
      //          if (Map[i][j] == WALL)
      //          {
      //              w_rect[w_rect_count].left = Board[i][j].left;
      //              w_rect[w_rect_count].right = Board[i][j].right;
      //              w_rect[w_rect_count].top = Board[i][j].top;
      //              w_rect[w_rect_count].bottom = Board[i][j].bottom-25;

      //              w_rect_count++;
      //          }
      //      }
      //  }

        break;

    case WM_TIMER:
        InvalidateRect(hWnd, NULL, true);
        //switch (wParam) {

        //case 1:
        //    player.accY = PLAYER_GRAVITY;
        //    player.accX = player.dx;
        //    player.velY += player.accY;


        //    // 0번 기능 - 키보드 입력에 따른 기능
        //    if (GetAsyncKeyState(VK_LEFT) & 0x8000)
        //    {
        //        player.accY = PLAYER_GRAVITY;
        //        if (player.state != STATE::JUMP) {
        //            player.ChangeState(STATE::MOVE);
        //        }

        //        player.dir = P_DIR_LEFT;
        //        player.x -= (300 * 0.016f);
        //    }

        //    if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
        //    {
        //        player.accY = PLAYER_GRAVITY;
        //        if (player.state != STATE::JUMP) {
        //            player.ChangeState(STATE::MOVE);
        //        }
        //        player.dir = P_DIR_RIGHT;
        //        player.x += (300 * 0.016f);
        //    }

        //    if (GetAsyncKeyState(VK_SPACE) & 0x8000 && player.isRanding)
        //    {
        //        player.isJump = true;
        //        player.isRanding = false;
        //        player.velY = -13.25f;
        //        player.ChangeState(STATE::JUMP);
        //    }


        //    if (player.y > 900) {
        //        player.y = 900;
        //        player.velY = 0;
        //        player.isJump = false;
        //        player.ChangeState(STATE::IDLE);
        //        player.Init();

        //    }
        //    else {
        //        player.y += player.velY + player.accY * 0.016f;

        //    }

        //    player.UpdateCollisionBox();
        //    player.UpdateBullet();


        //    //monster - player 충돌
        //    for (int i = 0; i < MONSTER_AMOUNT; ++i) {
        //        //플레이어 화면 범위 안에 있는 몬스터
        //        if (monsters[i].x - 20 > player.x - 500 && monsters[i].x +20 < player.x + 800 && !monsters[i].isDead) {

        //            if (CollisionHelper(monsters[i].collisionBox, player.collisionBox)) {
        //                player.Damaged();
        //            }

        //            // monster - bullet 충돌
        //            for (int j = 0; j < 10; ++j) {
        //                if (player.bullet[j].isAttack) {
        //                    if (CollisionHelper(player.bullet[j].collisionBox, monsters[i].collisionBox)) {
        //                        player.bullet[j].isAttack = false;
        //                        monsters[i].Damaged();
        //                    }
        //                }
        //            }
        //        }
        //    }




        //    for (int i = 0; i < 12; ++i) {
        //        if (CollisionHelper(button[i].collisionBox, player.collisionBox)) {

        //            if (button[i].collisionBox.top > player.collisionBox.bottom - 10 && player.velY >= 0)
        //            {
        //                if (player.state == STATE::JUMP || player.state == STATE::FALL) {
        //                    player.ChangeState(STATE::IDLE);
        //                }
        //                button[i].Collsion();
        //                player.y = button[i].collisionBox.top - 32;
        //                player.velY = 0;
        //                player.accY = 0;
        //                player.isRanding = true;
        //            }
        //        }
        //        else {
        //            button[i].SetImage();
        //        }
        //    }



        //    for (int i = 0; i < w_rect_count; ++i) {
        //        if (w_rect[i].right > player.x - 500 && w_rect[i].left < player.x + 800) {
        //            if (CollisionHelper(w_rect[i], player.collisionBox)) {
        //                //아래 충돌
        //                if (w_rect[i].top > player.collisionBox.bottom - 10 && player.velY >=0)
        //                {
        //                    if (player.state == STATE::JUMP || player.state == STATE::FALL) {
        //                        player.ChangeState(STATE::IDLE);
        //                    }
        //                    player.y = w_rect[i].top - 32;
        //                    player.velY = 0;
        //                    player.accY = 0;
        //                    player.isRanding = true;
        //                }
        //            }
        //        }
        //    }


        //    for (int i = 0; i < 3; ++i) {
        //        rever[i].Update();
        //    }

        //    for (int i = 0; i < MONSTER_AMOUNT; ++i) {
        //        monsters[i].Move();
        //        monsters[i].UpdateCollisionBox();
        //        monsters[i].UpdateAnimation();
        //    }
        //    player.UpdateAnimation();

        //    break;
        //}

        break;

    case WM_CHAR:
    /*    switch (wParam)
        {
        case 'a':
        case 'A':
            player.Shoot();
            break;
        case 'x':
        case 'X':
            for (int i = 0; i < 3; ++i) {
                if (CollisionHelper(rever[i].collisionBox, player.collisionBox)) {
                    rever[i].Collsion();
                }
            }
            break;
        }
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_SHIFT:
            break;
        }
        break;
    case WM_KEYUP:
        if (player.state != STATE::JUMP) {
            player.ChangeState(STATE::IDLE);
        }
        if (wParam == 'a' || wParam == 'A') {
            player.SetShoot(false);
        }
        break;*/

    case WM_PAINT:
        if (ID == -1) break;

        hdc = BeginPaint(hWnd, &ps);

        memdc1 = CreateCompatibleDC(hdc);
        hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);
        SelectObject(memdc1, hBitmap1);

        // 1. 배경 그리기

        if (ID > -1)
		{
            cout << ID;
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
					if (Board[i][j].right > player.x - 500 && Board[i][j].left < player.x + 800) {
						if (Map[i][j] == WALL)
						{
							Manager::GetInstance().wall.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 25);
						}

						if (Map[i][j] == MAGMA)
						{
							Manager::GetInstance().magma.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 25);
						}
					}
				}
			}

			for (int i = 0; i < MONSTER_AMOUNT; ++i) {
				if (monsters[i].collisionBox.right > player.x - 500 && monsters[i].collisionBox.left < player.x + 800) {
					if (!monsters[i].isDead) {

						monsters[i].draw_image.Draw(memdc1, monsters[i].x, monsters[i].y, 32, 32, monsters[i].anim, 32 * (int)monsters[i].m_type, 32, 32);
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
        else
        {
            cout << "sibal" << endl;
        }

        
        //for (int i = 0; i < 3; ++i) {
        //    rever[i].draw_image.Draw(memdc1, rever[i].x -25, rever[i].y, 50, 50);
        //}
        //
        //for (int i = 0; i < 6; ++i) {
        //    button[i].draw_image.Draw(memdc1, button[i].x - 25, button[i].y, 50, 25);
        //}



        // Player
        for (auto& player : players)
        {
            if (player.isDamaged) {
                if (player.Damaged_count % 2 == 0) {
                    player.getDrawImageByState().Draw(memdc1, player.x, player.y, 32, 32, player.anim, player.dir, 32, 32);
                }
            }
            else {
                player.getDrawImageByState().Draw(memdc1, player.x, player.y, 32, 32, player.anim, player.dir, 32, 32);
            }
        }
        
        for (auto& bullet : bullets) {
            if (bullet.second.isAttack) {
                //Rectangle(memdc1, player.bullet[i].collisionBox.left, player.bullet[i].collisionBox.top, player.bullet[i].collisionBox.right, player.bullet[i].collisionBox.bottom);
                Manager::GetInstance().bullet.Draw(memdc1, bullet.second.x, bullet.second.y,32,16);
            }
        }
        

        // 9. Heart 그리기

        // 10. 최종 Portal 그리기
        Manager::GetInstance().portal_img.Draw(memdc1, portal.x, portal.y, 52, 52, 0, 0, 52, 52);













        // --- RECT 테스트 ---

        //for (int i = 0; i < bullet_count; ++i) {
        //    Rectangle(memdc1, b_rect[i].left, b_rect[i].top, b_rect[i].right, b_rect[i].bottom);
        //}

        //for (int i = 0; i < MONSTER_AMOUNT; ++i) {
        //    Rectangle(memdc1, m_rect[i].left, m_rect[i].top, m_rect[i].right, m_rect[i].bottom);
        //}

        //for (int i = 0; i < OBS_SERO_COUNT; ++i) {
        //    Rectangle(memdc1, obs_s_rect[i].left, obs_s_rect[i].top, obs_s_rect[i].right, obs_s_rect[i].bottom);
        //}

        //for (int i = 0; i < OBS_GARO_COUNT; ++i) {
        //    Rectangle(memdc1, obs_g_rect[i].left, obs_g_rect[i].top, obs_g_rect[i].right, obs_g_rect[i].bottom);
        //}
        //Rectangle(memdc1, player.collisionBox.left, player.collisionBox.top, player.collisionBox.right, player.collisionBox.bottom);

        //for (int i = 0; i < w_rect_count; ++i) {            
        //    Rectangle(memdc1, w_rect[i].left, w_rect[i].top, w_rect[i].right, w_rect[i].bottom);
        //}

        // --- RECT 테스트 ---

        //if (player.x > 200) {
        //    BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, player.x - 200, 0, SRCCOPY);
        //}
        //else {
        //    BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, 0, 0, SRCCOPY);
        //}
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

bool CollisionHelper(RECT r1, RECT r2)
{
    if (r1.left > r2.right) return false;
    if (r1.top > r2.bottom) return false;
    if (r1.right < r2.left) return false;
    if (r1.bottom < r2.top) return false;

    return true;
}

