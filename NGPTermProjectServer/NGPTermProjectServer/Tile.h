#pragma once
#include "Object.h"

class Tile : Object
{
public:
	TileType type;
	bool isInteraction;

	Object* linkedObject;
public:
	void Interaction();
	virtual bool isCollision(Object* rhs);
};