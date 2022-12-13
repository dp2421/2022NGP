#include "stdafx.h"
#include "InteractionObejct.h"

InteractionObejct::InteractionObejct()
{

}

InteractionObejct::InteractionObejct(int x, int y, int left, int right, int top, int bottom, bool isinteraction)
{
	this->pos.x = x;
	this->pos.y = y;
	this->size.left = left;
	this->size.right = right;
	this->size.top = top;
	this->size.bottom = bottom;
	this->isInteraction = isinteraction;
}

InteractionObejct::~InteractionObejct()
{

}

void InteractionObejct::Update(float deltaTime)
{
	switch (this->type)
	{
	case ObjectType::Lever:
	{
		if (!isInteraction) return;
		int process = LeverResetTime - chrono::duration_cast<chrono::milliseconds>(std::chrono::high_resolution_clock::now() - interactionTime).count();
		if (process < 0)
		{
			isInteraction = false;
		}
		break;
	}
	case ObjectType::Door:
		break;
	case ObjectType::Button:
		break;
	case ObjectType::Potal:
		break;
	default:
		break;
	}
}

void InteractionObejct::Interaction()
{
	if (isInteraction) return;

	switch (this->type)
	{
	case ObjectType::Lever:
		for (auto& obj : linkedObjects)
			reinterpret_cast<InteractionObejct*>(obj)->Interaction();

		interactionTime = chrono::high_resolution_clock::now();
		break;
	case ObjectType::Door:
		for (auto& obj : linkedObjects)
		{
			if (!reinterpret_cast<InteractionObejct*>(obj)->isInteraction)
				return;
		}

		// ¹®¿­±â
		break;
	case ObjectType::Button:
		for (auto& obj : linkedObjects)
			reinterpret_cast<InteractionObejct*>(obj)->Interaction();

		break;
	case ObjectType::Potal:
		break;
	default:
		break;
	}
}
