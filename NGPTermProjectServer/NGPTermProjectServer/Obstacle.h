#pragma once
#include "Object.h"

class Obstacle : public Object
{
public:
	bool isActive = false;
public:
	Obstacle();
	~Obstacle();
	virtual void Update(float deltaTime) override;
};