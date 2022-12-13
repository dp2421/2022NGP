#pragma once
#include "Manager.h"

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
    int aniCount = 0;

    bool isShoot = false;
public:
    Player(int _x, int _y, CImage _draw_image = Manager::GetInstance().frog_idle);
    Player() {};
    void UpdateCollisionBox();
    void UpdateAnimation();

    void ChangeState(STATE _state) // cpp 파일로 옮기면 버그남 .. 이미지 로드 관련
    {
        if (state != _state)
        {
            anim = 0;
        }
        state = _state;
        switch (state)
        {
        case STATE::IDLE:
            image_count = 11;
            draw_image = Manager::GetInstance().frog_idle;
            break;
        case STATE::MOVE:
            image_count = 12;
            draw_image = Manager::GetInstance().frog_move;
            break;
        case STATE::JUMP:
            image_count = 1;
            draw_image = Manager::GetInstance().frog_jump;
            break;
        case STATE::FALL:
            image_count = 1;
            draw_image = Manager::GetInstance().frog_fall;
            break;
        case STATE::ATTACK:
            image_count = 7;
            draw_image = Manager::GetInstance().frog_attack;
            break;
        case STATE::DEAD:
            image_count = 6;
            draw_image = Manager::GetInstance().frog_attack;
            anim = 0;
            break;
        }
    }

    CImage& getDrawImageByState()
    {
        switch (this->state)
        {
        case STATE::IDLE:
            image_count = 11;
            return Manager::GetInstance().frog_idle;
        case STATE::MOVE:
            image_count = 12;
            return Manager::GetInstance().frog_move;
        case STATE::JUMP:
            image_count = 1;
            return Manager::GetInstance().frog_jump;
        case STATE::FALL:
            image_count = 1;
            return Manager::GetInstance().frog_fall;
        case STATE::ATTACK:
            image_count = 7;
            return Manager::GetInstance().frog_move;
        case STATE::DEAD:
            image_count = 6;
            return Manager::GetInstance().frog_attack;
        }
    }

    void Shoot();
    void SetShoot(bool _b) { isShoot = _b; }

    void UpdateBullet();
    void Init();
};