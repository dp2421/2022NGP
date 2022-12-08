#pragma once
#include "Object.h"

class Obstacle : public Object
{
public:
	int Speed;
	Vec2 velocity;
public:
	virtual void Update(float deltaTime) override;
};