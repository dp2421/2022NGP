#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <atlimage.h>
#include <string.h>

#include "Image.h"
#include "Map.h"
#include "State.h"
#include "Define.h"
#include "Obstacle.h"


HINSTANCE g_hInst;
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"Term Project";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

// 시간을 구하기 위한 변수들
LARGE_INTEGER g_tSecond;
LARGE_INTEGER g_tTime;
float         g_fDeltaTime;


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


enum class STATE {IDLE,MOVE,JUMP,FALL,ATTACK,DEAD};


struct Bullet {
    int x;
    int y;
    int dir;
    RECT collisionBox;
    bool isAttack = false;
    CImage draw_image;
};

enum class INTER_OBJ {REVER, BUTTON , PORTAL};
struct Interaction_Object {
    int x;
    int y;
    RECT collisionBox;
    bool isPulling = false;
    CImage draw_image;
    INTER_OBJ type;

    //나중에 시간함수로 바꾸3
    int current_time = 0;
    int max_time = 100;

    void SetCollsionBox()
    {
        collisionBox.left = x - 25;
        collisionBox.top = y;
        collisionBox.right = x + 25;
        collisionBox.bottom = y +  50;

    }

    void Update()
    {
        if (isPulling && type == INTER_OBJ::REVER) {
            if (current_time >= max_time) {
                isPulling = false;
                draw_image = rever_nomal;
            }
            else current_time++;
        }
    }


    void Collsion() {
        if (!isPulling) {
            isPulling = true;
            if (INTER_OBJ::REVER == type) {
                draw_image = rever_pull;
                current_time = 0;
            }
            if (INTER_OBJ::BUTTON == type) {
                draw_image = button_pull;
            }
        }
    }
    void SetImage()
    {
        draw_image = button_normal;
        isPulling = false;
    }
};

struct Potal
{

};





class Player {
public:
    int x;
    int y;
    int dir;
    int anim;
    int image_count;
    RECT collisionBox;
    Bullet bullet[10];
    STATE state;
    CImage draw_image;
    float dx, dy;
    float accX, accY;
    float velX, velY;

    int life = 5;

    bool isJump = false;
    bool isRanding = false;

    int bullet_count = 0;
    bool isDamaged = false;
    int Damaged_count = 0;

    bool isShoot = false;
public:
    void UpdateCollisionBox()
    {
        collisionBox.left = x+8;
        collisionBox.right = x + 25;
        collisionBox.top = y + 5;
        collisionBox.bottom = y + 32;
    }

    void UpdateAnimation()
    {
        if (isDamaged) {
        
            Damaged_count--;
            if (Damaged_count < 0) {
                isDamaged = false;
            }
        }


        if (velY > 0) {
            ChangeState(STATE::FALL);
        }

        anim += P_IMAGE_SIZE;
        if (anim >= P_IMAGE_SIZE * (image_count - 1)) {
            anim = 0;
            if (state == STATE::DEAD || state == STATE::ATTACK) {
                ChangeState(STATE::IDLE);
            }
        }
    }

    void ChangeState(STATE _state)
    {
        state = _state;
        switch (state)
        {
        case STATE::IDLE:
            image_count = 11;
            draw_image = frog_idle;
            break;
        case STATE::MOVE:
            image_count = 12;
            draw_image = frog_move;
            break;
        case STATE::JUMP:
            image_count = 1;
            draw_image = frog_jump;
            break;
        case STATE::FALL:
            image_count = 1;
            draw_image = frog_fall;
            break;
        case STATE::ATTACK:
            image_count = 7;
            draw_image = frog_attack;
            break;
        case STATE::DEAD:
            image_count = 6;
            draw_image = frog_attack;
            anim = 0;
            break;
        }
    }

    void Shoot()
    {
        if (!isShoot) {
            anim = 0;

            ChangeState(STATE::ATTACK);
            bullet[bullet_count].isAttack = true;
            if (dir == P_DIR_LEFT) {
                bullet[bullet_count].dir = -1;
            }
            else {
                bullet[bullet_count].dir = 1;
            }

            bullet[bullet_count].x = x;
            bullet[bullet_count].y = y + 10;
            bullet[bullet_count].collisionBox.left = bullet[bullet_count].x + 10;
            bullet[bullet_count].collisionBox.right = bullet[bullet_count].x + 26;
            bullet[bullet_count].collisionBox.top = bullet[bullet_count].y;
            bullet[bullet_count].collisionBox.bottom = bullet[bullet_count].y + 16;
            bullet_count++;
            if (bullet_count == 10) bullet_count = 0;
            isShoot = true;
        }
    }

    void SetShoot(bool _b)
    {
        isShoot = _b;
    }

    void UpdateBullet()
    {
        for (int i = 0; i < 10; ++i) {
            if (bullet[i].isAttack) {
                bullet[i].x += 10 * bullet[i].dir;
                bullet[i].collisionBox.left = bullet[i].x + 10;
                bullet[i].collisionBox.right = bullet[i].x + 26;
                bullet[i].collisionBox.top = bullet[i].y;
                bullet[i].collisionBox.bottom = bullet[i].y + 16;
            }
        }
    }

    void Init()
    {
        x = 230;
        y = 650;
        life = 5;
    }

    void Damaged()
    {
        if (life > 0) {
            if(!isDamaged){
                life--;
                Damaged_count = 10;
                isDamaged = true;
                ChangeState(STATE::DEAD);
            }
        }
        else {
            Init();
        }
    }
     
    
    Player(int _x, int _y, CImage _draw_image)
    {
        life = 5;
        draw_image = _draw_image;
        state = STATE::IDLE;
        image_count = 11;
        anim = 0;
        x = _x;
        y = _y;
        dir = 0;
        collisionBox.left = x + 5;
        collisionBox.right = x + 27;
        collisionBox.top = y;
        collisionBox.bottom = y + 32;

        dx = 0;
        dy = 0;
        accX = 0;
        accY = 0;
        velX = 0;
        velY = 0;
        


        for (int i = 0; i < 10; ++i) {

            bullet[i].x = x;
            bullet[i].y = y;
            bullet[i].collisionBox.left = bullet[i].x + 10;
            bullet[i].collisionBox.right = bullet[i].x + 26;
            bullet[i].collisionBox.top = bullet[i].y;
            bullet[i].collisionBox.bottom = bullet[i].y + 16;
            bullet[i].isAttack = false;
        }
    }
};

enum class MONSTER_TYPE { MONSTER_MOVE,  MONSTER_PEACE};

class Monster {
public:
    int x;
    int y;
    int dir;
    int anim;
    int image_count;
    RECT collisionBox;
    Bullet bullet[10];
    STATE state;
    CImage draw_image;
    float dx, dy;
    float accX, accY;
    float velX, velY;
    int life = 5;

    bool isJump = false;
    bool isRanding = false;
    bool isDead = false;

    int move_count = 0;
    MONSTER_TYPE m_type;
    

  
public:
    void UpdateCollisionBox()
    {
        collisionBox.left = x + 8;
        collisionBox.right = x + 25;
        collisionBox.top = y + 5;
        collisionBox.bottom = y + 32;
    }

    void UpdateAnimation()
    {
        anim += P_IMAGE_SIZE;
        if (anim >= P_IMAGE_SIZE * (image_count - 1))
            anim = 0;
    }

    void Move()
    {
        if (m_type == MONSTER_TYPE::MONSTER_MOVE) {
            move_count++;
            if (move_count == 10) {
                move_count = 0;
                dir *= -1;
            }
            x -= 10 * dir;
        }
    }

    void Damaged()
    {
        if (life > 1) {
            life--;
        }
        else {
            isDead = true;
        }
    }

    void ChangeState(STATE _state)
    {
        state = _state;
        switch (state)
        {
        case STATE::IDLE:
            image_count = 8;
            draw_image = monster_idle;
            break;
        case STATE::MOVE:
            image_count = 12;
            draw_image = frog_move;
            break;
        case STATE::JUMP:
            image_count = 1;
            draw_image = frog_jump;
            break;
        case STATE::FALL:
            image_count = 1;
            draw_image = frog_fall;
            break;
        case STATE::ATTACK:
            image_count = 7;
            draw_image = frog_attack;
            break;
        case STATE::DEAD:
            break;
        }
    }



    Monster(int _x, int _y, CImage _draw_image, MONSTER_TYPE _type)
    {
        life = 5;
        draw_image = _draw_image;
        state = STATE::IDLE;
        m_type = _type;
        image_count = 11;
        anim = 0;
        x = _x;
        y = _y;
        dir = 1;
        collisionBox.left = x + 5;
        collisionBox.right = x + 27;
        collisionBox.top = y;
        collisionBox.bottom = y + 32;

        dx = 0;
        dy = 0;
        accX = 0;
        accY = 0;
        velX = 0;
        velY = 0;

    }
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

    static Interaction_Object rever[2];
    static Interaction_Object button[2];
    static Interaction_Object portal;


    static Player player(8600, 420, frog_idle);


    static Monster monster[MONSTER_AMOUNT]{
    
        Monster(630, 670, monster_idle, MONSTER_TYPE::MONSTER_MOVE)
      , Monster(700, 420, monster_idle, MONSTER_TYPE::MONSTER_MOVE), Monster(400, 165, monster_idle, MONSTER_TYPE::MONSTER_MOVE)
      , Monster(970, 420, monster_idle,  MONSTER_TYPE::MONSTER_MOVE), Monster(1000, 670, monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(2300, 670, monster_idle, MONSTER_TYPE::MONSTER_MOVE), Monster(2400, 420, monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(2400, 165, monster_idle, MONSTER_TYPE::MONSTER_MOVE), Monster(2800, 165, monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(4300, 670, monster_idle, MONSTER_TYPE::MONSTER_MOVE), Monster(4500, 165, monster_idle,MONSTER_TYPE::MONSTER_MOVE)
      , Monster(4750, 420, monster_idle, MONSTER_TYPE::MONSTER_MOVE), Monster(5400, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(5550, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(5850, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(6150, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(6900, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7050, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(7050, 165, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7050, 670, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(7500, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7500, 170, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(7700, 670, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(7800, 625, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(7850, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8100, 170, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(8100, 625, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8300, 420, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(8700, 370, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8800, 270, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(8800, 720, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
      , Monster(8950, 320, monster_idle, MONSTER_TYPE::MONSTER_PEACE), Monster(8900, 165, monster_idle, MONSTER_TYPE::MONSTER_PEACE)
    };

    switch (uMsg) {

    case WM_CREATE:
        LoadImage();

        //레버 초기화
        rever[0].x = 400;
        rever[0].y = 650;
        rever[0].draw_image = rever_nomal;
        rever[1].x = 600;
        rever[1].y = 650;
        rever[1].draw_image = rever_nomal;
        rever[0].SetCollsionBox();
        rever[1].SetCollsionBox();
        rever[0].type = INTER_OBJ::REVER;
        rever[1].type = INTER_OBJ::REVER;


        //레버 초기화
       button[0].x = 500;
       button[0].y = 650;
       button[0].draw_image = button_normal;
       button[1].x = 700;
       button[1].y = 650;
       button[1].draw_image = button_normal;
       button[0].SetCollsionBox();
       button[1].SetCollsionBox();
       button[0].type = INTER_OBJ::BUTTON;
       button[1].type = INTER_OBJ::BUTTON;


       portal.x = 800;
       portal.y = 650;
       portal.draw_image = button_normal;
       portal.SetCollsionBox();
       portal.type = INTER_OBJ::PORTAL;




        player.ChangeState(STATE::IDLE);

        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            monster[i].ChangeState(STATE::IDLE);
        }
        SetTimer(hWnd, 1, 16, NULL);


        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                Board[i][j].left = 0 + j * BLOCK_SIZE;
                Board[i][j].right = 50 + j * BLOCK_SIZE;
                Board[i][j].top = 0 + i * BLOCK_SIZE;
                Board[i][j].bottom = 50 + i * BLOCK_SIZE;
            }
        }

        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                if (Map[i][j] == WALL)
                {
                    w_rect[w_rect_count].left = Board[i][j].left;
                    w_rect[w_rect_count].right = Board[i][j].right;
                    w_rect[w_rect_count].top = Board[i][j].top;
                    w_rect[w_rect_count].bottom = Board[i][j].bottom-25;

                    w_rect_count++;
                }
            }
        }


        break;

    case WM_TIMER:
        switch (wParam) {

        case 1:
            player.accY = PLAYER_GRAVITY;
            player.accX = player.dx;
            player.velY += player.accY;


            // 0번 기능 - 키보드 입력에 따른 기능
            if (GetAsyncKeyState(VK_LEFT) & 0x8000)
            {
                player.accY = PLAYER_GRAVITY;
                if (player.state != STATE::JUMP) {
                    player.ChangeState(STATE::MOVE);
                }

                player.dir = P_DIR_LEFT;
                player.x -= (300 * 0.016f);
            }

            if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
            {
                player.accY = PLAYER_GRAVITY;
                if (player.state != STATE::JUMP) {
                    player.ChangeState(STATE::MOVE);
                }
                player.dir = P_DIR_RIGHT;
                player.x += (300 * 0.016f);
            }

            if (GetAsyncKeyState(VK_SPACE) & 0x8000 && player.isRanding)
            {
                player.isJump = true;
                player.isRanding = false;
                player.velY = -13.25f;
                player.ChangeState(STATE::JUMP);
            }


            if (player.y > 900) {
                player.y = 900;
                player.velY = 0;
                player.isJump = false;
                player.ChangeState(STATE::IDLE);
                player.Init();

            }
            else {
                player.y += player.velY + player.accY * 0.016f;

            }

            player.UpdateCollisionBox();
            player.UpdateBullet();


            //monster - player 충돌
            for (int i = 0; i < MONSTER_AMOUNT; ++i) {
                //플레이어 화면 범위 안에 있는 몬스터
                if (monster[i].x - 20 > player.x - 500 && monster[i].x +20 < player.x + 800 && !monster[i].isDead) {

                    if (CollisionHelper(monster[i].collisionBox, player.collisionBox)) {
                        player.Damaged();
                    }

                    // monster - bullet 충돌
                    for (int j = 0; j < 10; ++j) {
                        if (player.bullet[j].isAttack) {
                            if (CollisionHelper(player.bullet[j].collisionBox, monster[i].collisionBox)) {
                                player.bullet[j].isAttack = false;
                                monster[i].Damaged();
                            }
                        }
                    }
                }
            }




            for (int i = 0; i < 2; ++i) {
                if (CollisionHelper(button[i].collisionBox, player.collisionBox)) {

                    if (button[i].collisionBox.top > player.collisionBox.bottom - 10 && player.velY >= 0)
                    {
                        if (player.state == STATE::JUMP || player.state == STATE::FALL) {
                            player.ChangeState(STATE::IDLE);
                        }
                        button[i].Collsion();
                        player.y = button[i].collisionBox.top - 32;
                        player.velY = 0;
                        player.accY = 0;
                        player.isRanding = true;
                    }
                }
                else {
                    button[i].SetImage();
                }
            }



            for (int i = 0; i < w_rect_count; ++i) {
                if (w_rect[i].right > player.x - 500 && w_rect[i].left < player.x + 800) {
                    if (CollisionHelper(w_rect[i], player.collisionBox)) {
                        //아래 충돌
                        if (w_rect[i].top > player.collisionBox.bottom - 10 && player.velY >=0)
                        {
                            if (player.state == STATE::JUMP || player.state == STATE::FALL) {
                                player.ChangeState(STATE::IDLE);
                            }
                            player.y = w_rect[i].top - 32;
                            player.velY = 0;
                            player.accY = 0;
                            player.isRanding = true;
                        }
                    }
                }
            }


            for (int i = 0; i < 2; ++i) {
                rever[i].Update();
            }

            for (int i = 0; i < MONSTER_AMOUNT; ++i) {
                monster[i].Move();
                monster[i].UpdateCollisionBox();
                monster[i].UpdateAnimation();
            }
            player.UpdateAnimation();

            break;
        }

        InvalidateRect(hWnd, NULL, false);
        break;

    case WM_CHAR:
        switch (wParam)
        {
        case 'a':
        case 'A':
            player.Shoot();
            break;
        case 'x':
        case 'X':
            for (int i = 0; i < 2; ++i) {
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
        break;

    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);

        memdc1 = CreateCompatibleDC(hdc);
        hBitmap1 = CreateCompatibleBitmap(hdc, 11500, Window_Size_Y);
        SelectObject(memdc1, hBitmap1);

        // 1. 배경 그리기

        if (player.x >= 200) {
            bg.Draw(memdc1, -Window_Size_X + scroll_x + player.x - 200, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
            bg.Draw(memdc1, scroll_x + player.x - 200, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
        }
        else {
            bg.Draw(memdc1, -Window_Size_X + scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
            bg.Draw(memdc1, scroll_x, 0, Window_Size_X, Window_Size_Y, 0, 0, bg_width, bg_height);
        }

        // 2. 발판 그리기
        for (int i = 0; i < RAW; ++i) {
            for (int j = 0; j < COLUMN; ++j) {
                if (Board[i][j].right > player.x - 500 && Board[i][j].left < player.x + 800) {
                    if (Map[i][j] == WALL)
                    {
                        wall.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 25);
                    }

                    if (Map[i][j] == MAGMA)
                    {
                        magma.Draw(memdc1, Board[i][j].left, Board[i][j].top, 50, 25);
                    }
                }
            }
        }

        for (int i = 0; i < MONSTER_AMOUNT; ++i) {
            if (monster[i].collisionBox.right > player.x - 500 && monster[i].collisionBox.left < player.x + 800) {
                if (!monster[i].isDead) {

                    monster[i].draw_image.Draw(memdc1, monster[i].x, monster[i].y, 32, 32, monster[i].anim,  32 *(int)monster[i].m_type, 32, 32);
                }
            }
        }

        
        for (int i = 0; i < 2; ++i) {
            rever[i].draw_image.Draw(memdc1, rever[i].x -25, rever[i].y, 50, 50);
        }

        for (int i = 0; i < 2; ++i) {
            button[i].draw_image.Draw(memdc1, button[i].x - 25, button[i].y, 50, 25);
        }



        // Player
        if (player.isDamaged) {
            if (player.Damaged_count % 2 == 0) {
                player.draw_image.Draw(memdc1, player.x, player.y, 32, 32, player.anim, player.dir, 32, 32);
            }
        }
        else {
            player.draw_image.Draw(memdc1, player.x, player.y, 32, 32, player.anim, player.dir, 32, 32);
        }
        
        for (int i = 0; i < 10; ++i) {
            if (player.bullet[i].isAttack) {
                //Rectangle(memdc1, player.bullet[i].collisionBox.left, player.bullet[i].collisionBox.top, player.bullet[i].collisionBox.right, player.bullet[i].collisionBox.bottom);
                bullet.Draw(memdc1,player.bullet[i].x, player.bullet[i].y,32,16);
            }
        }
        //


        // 9. Heart 그리기
        for (int i = 0; i < player.life; ++i) {
            if (player.x > 200) {
                My_heart.Draw(memdc1, 30 + 45 * i + player.x - 200, 20, 45, 46, 0, 0, 45, 46);
            }
            else {
                My_heart.Draw(memdc1, 30 + 45 * i + 0, 20, 45, 46, 0, 0, 45, 46);
            }
        }

        // 10. 최종 Portal 그리기
        portal_img.Draw(memdc1, portal.x, portal.y, 52, 52, 0, 0, 52, 52);















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

        if (player.x > 200) {
            BitBlt(hdc, 0, 0, Window_Size_X, Window_Size_Y, memdc1, player.x - 200, 0, SRCCOPY);
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
    bg.Load(L"background.png");
    bg_width = bg.GetWidth();
    bg_height = bg.GetHeight();

    wall.Load(L"block_wall.png");

    monster_idle.Load(L"pacman.png");
    monster_dead.Load(L"monster_dead.png");

    frog_idle.Load(L"frog_idle.png");
    frog_move.Load(L"frog_move.png");
    frog_jump.Load(L"frog_jump.png");
    frog_fall.Load(L"frog_fall.png");
    frog_attack.Load(L"frog_attack.png");

    bullet.Load(L"bullet.png");

    effect.Load(L"effect.png");

    magma.Load(L"magma.jpg");

    garo.Load(L"garo.png");
    sero.Load(L"sero.png");

    My_heart.Load(L"heart.png");

    portal_img.Load(L"portal.png");

    FailGame.Load(L"FailGame.png");
    SuccessGame.Load(L"SuccessGame.png");

    rever_nomal.Load(L"Rever1.png");
    rever_pull.Load(L"Rever2.png");
    button_normal.Load(L"Button1.png");
    button_pull.Load(L"Button2.png");
}

bool CollisionHelper(RECT r1, RECT r2)
{
    if (r1.left > r2.right) return false;
    if (r1.top > r2.bottom) return false;
    if (r1.right < r2.left) return false;
    if (r1.bottom < r2.top) return false;

    return true;
}

