#pragma once
#include "Object.h"

class Obstacle : Object
{
public:
	int Speed;
	Vec2 velocity;
public:
	virtual void Update(float deltaTime) override;
};