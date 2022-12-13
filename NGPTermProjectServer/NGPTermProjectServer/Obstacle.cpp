#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle()
{
	this->size.left = -25;
	this->size.right = +25;
	this->size.top = -25;
	this->size.bottom = 25;
}

Obstacle::~Obstacle()
{

}

void Obstacle::Update(float deltaTime)
{
	if (!this->isActive) return;

	this->pos.x -= ObstacleSpeed * deltaTime;

	if (this->pos.x < -100) this->isActive = false;
}