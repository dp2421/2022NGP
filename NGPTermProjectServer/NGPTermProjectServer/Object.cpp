#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

bool Object::isCollision(Object* rhs)
{
	if (this->pos.x + this->size.left > rhs->pos.x + rhs->size.right ||
		this->pos.y + this->size.top > rhs->pos.y + rhs->size.bottom ||
		this->pos.x + this->size.right < rhs->pos.x + rhs->size.left ||
		this->pos.y + this->size.bottom < rhs->pos.y + rhs->size.top)
		return false;

	return true;
}

bool Object::isCollision(Vec2 pos, RECT size)
{
	if (this->pos.x + this->size.left > pos.x + size.right ||
		this->pos.y + this->size.top > pos.y + size.bottom ||
		this->pos.x + this->size.right < pos.x + size.left ||
		this->pos.y + this->size.bottom < pos.y + size.top)
		return false;

	return true;
}

void Object::Interaction()
{
	
}

//if (r1.left > r2.right) return false;
//if (r1.top > r2.bottom) return false;
//if (r1.right < r2.left) return false;
//if (r1.bottom < r2.top) return false;