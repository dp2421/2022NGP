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
	if (this->pos.x - this->size.x / 2 > rhs->pos.x + rhs->size.x / 2 ||
		this->pos.x + this->size.x / 2 < rhs->pos.x - rhs->size.x / 2 ||
		this->pos.y - this->size.y / 2 > rhs->pos.y + rhs->size.y / 2 ||
		this->pos.y + this->size.y / 2 < rhs->pos.y - rhs->size.y / 2)
		return true;

	return false;
}
