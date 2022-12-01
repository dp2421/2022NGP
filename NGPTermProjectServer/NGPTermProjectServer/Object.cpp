#include "Object.h"
#include "stdafx.h"

Object::Object()
{
}

Object::~Object()
{
}

bool Object::isCollision(Object* rhs)
{
	if (this->pos.x + this->size.left > rhs->pos.x + rhs->size.right ||
		this->pos.x + this->size.top > rhs->pos.x + rhs->size.bottom ||
		this->pos.y + this->size.right < rhs->pos.y + rhs->size.left ||
		this->pos.y + this->size.bottom < rhs->pos.y + rhs->size.top)
		return false;

	return true;
}

bool Object::isCollision(Vec2 pos, RECT size)
{
	if (this->pos.x + this->size.left > pos.x + size.right ||
		this->pos.x + this->size.top > pos.x + size.bottom ||
		this->pos.y + this->size.right < pos.y + size.left ||
		this->pos.y + this->size.bottom < pos.y + size.top)
		return false;

	return true;
}