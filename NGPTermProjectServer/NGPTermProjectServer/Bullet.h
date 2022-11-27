#pragma once
#include "Object.h"

class Bullet : Object
{
public:
	bool isActive;
	int direction;
	int speed;

public:
	Bullet();
	~Bullet();

	virtual void Update(float deltaTime) override;

	void Shot();
};
