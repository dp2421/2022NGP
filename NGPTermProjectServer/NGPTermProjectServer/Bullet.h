#pragma once
#include "Object.h"

class Bullet : Object
{
public:
	bool isActive = false;
	int direction = 0;

public:
	Bullet();
	~Bullet();

	virtual void Update(float deltaTime) override;
	void Shot(Vec2 vec, int dir);
	void CollisionMonster();
};
