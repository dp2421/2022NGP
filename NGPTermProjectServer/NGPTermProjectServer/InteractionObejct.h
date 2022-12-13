#pragma once
#include "Object.h"

class InteractionObejct : public Object
{
public:
	chrono::steady_clock::time_point interactionTime;
	bool isInteraction = false;
	ObjectType type;

	deque<Object*> linkedObjects;
public:
	InteractionObejct();
	InteractionObejct(int x, int y, int left, int right, int top, int bottom, bool isinteraction);
	~InteractionObejct();

	virtual void Update(float deltaTime) override;

	virtual void Interaction() override;
};