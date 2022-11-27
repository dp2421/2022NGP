#pragma once
#include "Object.h"

class Bullet : Object
{
public:
	bool isActive;
	int direction;
	int speed;

public:
	virtual void Update(float deltaTime) override;
	virtual bool isCollision(Object* rhs) override;
};
