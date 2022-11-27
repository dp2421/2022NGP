#pragma once
#include "Object.h"

class Monster : Object
{
public:
	int HP;
	int direction;
	int speed;
public:
	Monster();
	~Monster();

	virtual void Update(float deltaTime) override;
};
