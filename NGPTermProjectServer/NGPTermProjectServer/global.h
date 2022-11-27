#pragma once
#include "stdafx.h"

struct Vec2
{
	float x = 0.f, y = 0.f;
};

enum class PlayerState : int
{
	None = 0,
	Idle = 1,
	Attack = 1 << 1,
	Jump = 1 << 2,
	Left = 1 << 3,
	Right = 1 << 4,
	Dead = 1 << 5,
	End
};

enum class TileType : int
{
	Wall,
	Magma,
	End
};

constexpr int startCountdownTime = 5000;

constexpr float playerSpeed = 300;
constexpr float  PLAYER_GRAVITY = 0.9f;

constexpr int BlockSize = 50;

constexpr int BulletMaxDistance = 10000;

using namespace std;