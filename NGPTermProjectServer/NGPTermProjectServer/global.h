#pragma once
#include "stdafx.h"

enum class STATE { IDLE, MOVE, JUMP, FALL, ATTACK, DEAD };

struct Vec2
{
	Vec2() { }
	Vec2(float x, float y) : x(x), y(y) {}
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
	Wall = 1,
	Magma,
	End
};

constexpr int maxBulletCount = 100;
constexpr int maxObstacleCount = 5;

constexpr int startCountdownTime = 5000;

constexpr float playerSpeed = 300;
constexpr float  PLAYER_GRAVITY = 5.0f;

constexpr float MonsterSpeed = 100;

constexpr float BulletSpeed = 10;

constexpr int BlockSize = 50;

constexpr int BulletMaxDistance = 20000;

using namespace std;