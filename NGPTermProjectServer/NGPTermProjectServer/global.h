#pragma once
#include "stdafx.h"

class Vec2
{
	float x, y;
};

enum class PlayerState : int
{
	Idle,
	Move,
	Attack,
	Dead,
	End
};

enum class TileType : int
{
	Wall,
	Magma,
	End
};

constexpr int countTime = 5000;

using namespace std;