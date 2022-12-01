#pragma once
#include "global.h"

class Object
{
public:
	Vec2 pos;
	RECT size;
public:
	Object();
	~Object();

	virtual void Update(float deltaTime) = 0;
	bool isCollision(Object* rhs);
	bool isCollision(Vec2 pos, RECT size);
};
