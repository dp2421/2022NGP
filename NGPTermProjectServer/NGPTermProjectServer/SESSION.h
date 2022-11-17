#pragma once
#include "stdafx.h"
#include "Player.h"

class SESSION
{
	int ID;
	SOCKET socket;
	Player player;

	void DoSend(void* packet);
	void DoRevc();
};

