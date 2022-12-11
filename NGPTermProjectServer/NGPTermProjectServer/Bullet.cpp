#include "stdafx.h"
#include "Bullet.h"
#include "GameManager.h"
#include "Monster.h"

Bullet::Bullet()
{
    this->isActive = false;
    this->direction = 0;

    this->size.left = 10;
    this->size.right = 26;
    this->size.top = 0;
    this->size.bottom = 16;
}

Bullet::~Bullet()
{
}

void Bullet::Update(float deltaTime)
{
    if (!isActive) return;

    this->pos.x += BulletSpeed * this->direction * deltaTime;
    if (abs(this->pos.x) > BulletMaxDistance) isActive = false;
}

void Bullet::Shot(Vec2 vec, int dir)
{
    this->direction = dir;
    this->pos.x = vec.x;
    this->pos.y = vec.y + 10;
    this->isActive = true;
}

void Bullet::CollisionMonster()
{
    auto& monsters = GameManager::GetInstance().monsters;
    for (auto& monster : monsters)
    {
        if (this->isCollision(monster)) 
        {
            reinterpret_cast<Monster*>(monster)->Damaged();
            this->isActive = false;
            return;
        }
    }
}