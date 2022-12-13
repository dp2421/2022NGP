#include "stdafx.h"
#include "Monster.h"

Monster::Monster()
{
    this->HP = 5;
    this->moveCount = 0;
    this->size.left = 8;
    this->size.right = 25;
    this->size.top = 5;
    this->size.bottom = 32;
}

Monster::~Monster()
{
}

void Monster::Update(float deltaTime)
{
    if (this->HP <= 0) return;

    this->moveCount++;
    if (this->moveCount == 10) {
        this->moveCount = 0;
        this->direction *= -1;
    }
    this->pos.x -= MonsterSpeed * this->direction * deltaTime;
}

void Monster::Damaged()
{
    this->HP--;
    if(this->HP <= 0) {
        //Á×À½
    }
}