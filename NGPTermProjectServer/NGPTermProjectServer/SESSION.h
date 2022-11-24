#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Protocol.h"

class SESSION
{
public:
	int ID;
	SOCKET socket;
	sockaddr_in sockAddr;
	char addr[INET_ADDRSTRLEN];

	Player player;

public:
	void DoSend(void* packet);
	void DoRevc();

	void SendLoginPacket();
	void SendCountdownPacket(char cnt);
	void SendGameStartPacket();
	void SendMapInfoPacket(char* map);
	void SendGameStartPacket();
	void SendMapInfoPacket();
	void SendTileInfoPacket();
	void SendPlayerInfoPacket();
	void SendMonsterInfoPacket();
	void SendBulletInfoPakcet();	
	void SendObstacleInfoPacket();
	void SendGameClearPacket();
};