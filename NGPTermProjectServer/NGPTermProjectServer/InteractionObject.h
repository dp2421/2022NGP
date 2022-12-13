#pragma once
#include "Object.h"

class InteractionObject : public Object
{
public:
	chrono::steady_clock::time_point interactionTime;
	bool isInteraction = false;
	ObjectType type;

	int collisionPlayer = -1;

	deque<Object*> linkedObjects;
public:
	InteractionObject();
	InteractionObject(int x, int y, int left, int right, int top, int bottom, ObjectType type);
	~InteractionObject();

	virtual void Update(float deltaTime) override;

	virtual void Interaction() override;
};