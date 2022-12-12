#pragma once
#include "Object.h"

class Monster : public Object
{
public:
	int HP = 5;
	int direction = 1;
public:
	Monster();
	~Monster();

	virtual void Update(float deltaTime) override;
	void Damaged();
private:
	int moveCount;
};