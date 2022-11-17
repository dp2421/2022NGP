#pragma once
#include "global.h"

class Object
{
public:
	Vec2 vec;
	Vec2 size;
public:
	virtual void Update(float deltaTime) = 0;
	virtual bool isCollision(Object* rhs);
};
