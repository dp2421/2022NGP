#include "stdafx.h"
#include "Player.h"


Player::Player(int _x, int _y, CImage _draw_image)
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

void Player::Damaged()
{
    if (life > 0) {
        if (!isDamaged) {
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

void Player::Init()
{
    x = 230;
    y = 650;
    life = 5;
}

void Player::UpdateBullet()
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

void Player::Shoot()
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

void Player::UpdateAnimation()
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

void Player::UpdateCollisionBox()
{
    collisionBox.left = x + 8;
    collisionBox.right = x + 25;
    collisionBox.top = y + 5;
    collisionBox.bottom = y + 32;
}