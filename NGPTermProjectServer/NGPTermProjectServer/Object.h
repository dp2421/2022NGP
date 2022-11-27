#pragma once
#include "global.h"

class Object
{
public:
	Vec2 pos;
	Vec2 size;
public:
	Object();
	~Object();

	virtual void Update(float deltaTime) = 0;
	bool isCollision(Object* rhs);
};
