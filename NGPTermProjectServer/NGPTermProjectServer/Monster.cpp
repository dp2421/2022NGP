#include "stdafx.h"
#include "Monster.h"

Monster::Monster()
{
    this->HP = 5;
    this->moveCount = 0;
}

Monster::~Monster()
{
}

void Monster::Update(float deltaTime)
{
    if (this->HP < 1) return;

    this->moveCount++;
    if (this->moveCount == 10) {
        this->moveCount = 0;
        this->direction *= -1;
    }
    this->pos.x -= MonsterSpeed * this->direction * deltaTime;
}

void Monster::Damaged()
{
    if (this->HP > 1) {
        this->HP--;
    }
    else {
        //Á×À½
    }
}