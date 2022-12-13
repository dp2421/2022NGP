#include "stdafx.h"
#include "InteractionObject.h"

InteractionObject::InteractionObject()
{

}

InteractionObject::InteractionObject(int x, int y, int left, int right, int top, int bottom, ObjectType type)
{
	this->pos.x = x;
	this->pos.y = y;
	this->size.left = left;
	this->size.right = right;
	this->size.top = top;
	this->size.bottom = bottom;
	this->type = type;
}

InteractionObject::~InteractionObject()
{

}

void InteractionObject::Update(float deltaTime)
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

void InteractionObject::Interaction()
{
	switch (this->type)
	{
	case ObjectType::Lever:
		if (isInteraction) return;

		this->isInteraction = true;
		for (auto& obj : this->linkedObjects)
			reinterpret_cast<InteractionObject*>(obj)->Interaction();

		interactionTime = chrono::high_resolution_clock::now();
		break;
	case ObjectType::Door:
		if (isInteraction) return;

		for (auto& obj : this->linkedObjects)
		{
			if (!reinterpret_cast<InteractionObject*>(obj)->isInteraction)
				return;
		}
		
		this->isInteraction = true;
		// ¹®¿­±â
		break;
	case ObjectType::Button:
		this->isInteraction = !this->isInteraction;
		for (auto& obj : this->linkedObjects)
			reinterpret_cast<InteractionObject*>(obj)->Interaction();

		break;
	case ObjectType::Potal:
		break;
	default:
		break;
	}
}
