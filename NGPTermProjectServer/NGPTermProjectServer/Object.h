#pragma once
#include "global.h"

class Object
{
public:
	char ID;

	Vec2 pos;
	RECT size;
public:
	Object();
	~Object();

	virtual void Update(float deltaTime) = 0;
	bool isCollision(Object* rhs);
	bool isCollision(Vec2 pos, RECT size);

	virtual void Interaction();
};
