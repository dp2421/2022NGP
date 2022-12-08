#pragma once
#include "stdafx.h"
#include "Player.h"
#include "Protocol.h"
#include "Monster.h"
#include "Bullet.h"
#include "Obstacle.h"

class SESSION
{
public:
	int ID;
	SOCKET socket;
	sockaddr_in sockAddr;
	char addr[INET_ADDRSTRLEN];

	Player player;

public:
	void DoSend(InfoOfPacket* info, void* packet);
	void DoRevc();

	void SendLoginPacket();
	void SendCountdownPacket(char cnt);
	void SendGameStartPacket();
	void SendMapInfoPacket(char* map);
	void SendTileInfoPacket();
	void SendPlayerInfoPacket(SESSION& player);
	void SendMonsterInfoPacket(Monster* monster);
	void SendBulletInfoPakcet(Bullet* bullet);	
	void SendObstacleInfoPacket(Obstacle* obstacle);
	void SendGameClearPacket(chrono::seconds time);
};

int RecvExpasion(SOCKET sock, char* buf, int len, int flage);

void SendExpansion(SOCKET sock, char* buf, int len, int flage);

void err_quit(const char* msg);

void err_display(const char* msg);

void err_display(int errcode);