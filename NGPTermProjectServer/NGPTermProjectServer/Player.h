#pragma once
#include "Object.h"

class Player : Object
{
public:
	PlayerState state;
	bool isJump;
	bool isGround;

	int HP;
	Vec2 velocity;
public:
	virtual void Update(float deltaTime) override;
	virtual bool isCollision(Object* rhs);

	void ProccesInput(int key);

	void Move();
	void Jump();
	void Interaction();
};