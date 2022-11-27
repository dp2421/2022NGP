#pragma once
#include "stdafx.h"
#include "Object.h"
#include "SESSION.h"
#include "Bullet.h"

class GameManager
{
public:
	deque<Object*> bullets;
	deque<Object*> monsters;
	SESSION clients[3];

private:
	GameManager() {}
	GameManager(const GameManager& ref) {}
	GameManager& operator=(const GameManager& ref) {}
	~GameManager() {}
public:
	static GameManager& GetInstance()
	{
		static GameManager instance;
		return instance;
	}
};