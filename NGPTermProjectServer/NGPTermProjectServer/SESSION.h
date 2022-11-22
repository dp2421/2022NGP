#pragma once
#include "stdafx.h"
#include "Player.h"

class SESSION
{
public:
	int ID;
	SOCKET socket;
	Player player;

public:
	void DoSend(void* packet);
	void DoRevc();
};


void SendLoginPacket(int id)
{
	Server2ClientLoginPacket p;
	p.size = sizeof(Server2ClientLoginPacket);
	p.type = Server2ClientLogin;
	p.ID = id;
}

void SendCountdownPacket(char cnt)
{
	Server2ClientCountdownPacket p;
	p.size = sizeof(Server2ClientCountdownPacket);
	p.type = Server2ClientCountdown;
	p.count = cnt;
}

void SendGameStartPacket()
{
	Server2ClientGameStartPacket p;
	p.size = sizeof(Server2ClientGameStartPacket);
	p.type = Server2ClientGameStart;

}

void SendMapInfoPacket(int id)
{
	Server2ClientMapInfoPacket p;
	p.size = sizeof(Server2ClientMapInfoPacket);
	p.type = Server2ClientMapInfo;
	p.width = MAPWIDTH;
	p.height = MAPHEIGHT;
	memcpy(p.mapInfo, Map, sizeof(Map));
}

void SendGameStartPacket(int id)
{

}