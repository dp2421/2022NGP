#pragma once
#include "Manager.h"
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

    Monster(int _x, int _y, CImage _draw_image, MONSTER_TYPE _type);

    void UpdateCollisionBox();
    void UpdateAnimation();

    void Move();
    void Damaged();

    void ChangeState(STATE _state)
    {
        state = _state;
        switch (state)
        {
        case STATE::IDLE:
            image_count = 8;
            draw_image = Manager::GetInstance().monster_idle;
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
            break;
        }
    }



};