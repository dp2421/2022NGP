#pragma once

class Vec2
{
	float x, y;
};

enum PlayerState
{
	Idle,
	Move,
	Attack,
	Dead,
	End
};

enum TileType
{
	Wall,
	Magma,
	Plate,
	Lever,
	Door,
	Potal,
	End
};