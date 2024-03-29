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
	char ID;
	SOCKET socket;

	Player player;

	char buffer[5000];
	char sendBuffer[BUFFERSIZE];
public:
	SESSION() {}
	~SESSION() {}

	void DoSend(InfoOfPacket* info, void* packet);
	int DoRevc(void* packet, int size);

	void SendLoginPacket();
	void SendCountdownPacket(char cnt);
	void SendGameStartPacket();
	void SendMapInfoPacket();
	void SendInteractionObjectInfoPacket();
	void SendPlayerInfoPacket(SESSION& player);
	void SendMonsterInfoPacket();
	void SendBulletInfoPakcet();	
	void SendObstacleInfoPacket();
	void SendGameClearPacket(chrono::seconds time);
};

int RecvExpasion(SOCKET sock, void* buf, int len, int flage);

void SendExpansion(SOCKET sock, void* buf, int len, int flage);

void err_quit(const char* msg);

void err_display(const char* msg);

void err_display(int errcode);