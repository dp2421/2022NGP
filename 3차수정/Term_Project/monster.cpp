#include "stdafx.h"
#include "Monster.h"

Monster::Monster(int _x, int _y, CImage _draw_image, MONSTER_TYPE _type)
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

void Monster::UpdateCollisionBox()
{
    collisionBox.left = x + 8;
    collisionBox.right = x + 25;
    collisionBox.top = y + 5;
    collisionBox.bottom = y + 32;
}

void Monster::Damaged()
{
    if (life > 1) {
        life--;
    }
    else {
        isDead = true;
    }
}

void Monster::Move()
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


void Monster::UpdateAnimation()
{
    anim += P_IMAGE_SIZE;
    if (anim >= P_IMAGE_SIZE * (image_count - 1))
        anim = 0;
}
