#include "stdafx.h"
#include "SESSION.h"

void SESSION::DoSend(InfoOfPacket* info, void* packet)
{
	SendExpansion(this->socket, (char*)info, sizeof(info), 0);
	if(packet != NULL)
		SendExpansion(this->socket, (char*)packet, info->size, 0);
}

void SESSION::DoRevc()
{

}

void SESSION::SendLoginPacket()
{
	Server2ClientLoginPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientLoginPacket);
	info.type = Server2ClientLogin;
	p.ID = this->ID;

	this->DoSend(&info, &p);
}

void SESSION::SendCountdownPacket(char cnt)
{
	Server2ClientCountdownPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientCountdownPacket);
	info.type = Server2ClientCountdown;
	p.count = cnt;

	this->DoSend(&info, &p);
}

void SESSION::SendGameStartPacket()
{
	InfoOfPacket info;
	info.size = sizeof(InfoOfPacket);
	info.type = Server2ClientGameStart;

	this->DoSend(&info, NULL);
}

void SESSION::SendMapInfoPacket(char* map)
{
	Server2ClientMapInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientMapInfoPacket);
	info.type = Server2ClientMapInfo;
	p.width = MAPWIDTH;
	p.height = MAPHEIGHT;
	memcpy(p.mapInfo, map, sizeof(map));

	this->DoSend(&info, &p);
}

void SESSION::SendTileInfoPacket()
{

}

void SESSION::SendPlayerInfoPacket(SESSION& player)
{
	Server2ClientPlayerInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientPlayerInfoPacket);
	info.type = Server2ClientPlayerInfo;

	p.ID = player.ID;
	p.state = player.player.state;
	p.HP = player.player.HP;
	p.x = player.player.pos.x;
	p.y = player.player.pos.y;

	this->DoSend(&info, &p);
}

void SESSION::SendMonsterInfoPacket(Monster* monster)
{
	Server2ClientMonsterInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientMonsterInfoPacket);
	info.type = Server2ClientMonsterInfo;

	p.HP = monster->HP;
	p.x = monster->pos.x;
	p.y = monster->pos.y;

	this->DoSend(&info, &p);
}

void SESSION::SendBulletInfoPakcet(Bullet* bullet)
{
	Server2ClientBulletInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientBulletInfoPacket);
	info.type = Server2ClientBulletInfo;

	p.x = bullet->pos.x;
	p.y = bullet->pos.y;

	this->DoSend(&info, &p);
}

void SESSION::SendObstacleInfoPacket(Obstacle* obstacle)
{
	Server2ClientObstacleInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientObstacleInfoPacket);
	info.type = Server2ClientObstacleInfo;

	p.veloX = obstacle->velocity.x;
	p.veloY = obstacle->velocity.y
	p.x = obstacle->pos.x;
	p.y = obstacle->pos.y;

	this->DoSend(&info, &p);
}

void SESSION::SendGameClearPacket(chrono::seconds time)
{
	Server2ClientGameClearPacket p;
	InfoOfPacket info;

	p.second = static_cast<int>(time.count());
}

int RecvExpasion(SOCKET sock, char* buf, int len, int flage)
{
	int retval = recv(sock, (char*)&buf, len, flage);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	else return retval;
}

void SendExpansion(SOCKET sock, char* buf, int len, int flage)
{
	if (len > BUFFERSIZE)
	{
		while (len != 0)
		{
			int sendSize = len > BUFFERSIZE ? BUFFERSIZE : len;
			int retval = send(sock, (char*)&buf, sendSize, flage);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
			}

			len -= BUFFERSIZE;
		}
	}
	else
	{
		int sendSize = len > BUFFERSIZE ? BUFFERSIZE : len;
		int retval = send(sock, (char*)&buf, len, flage);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
	}
}

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 소켓 함수 오류 출력`
void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[오류] %s\n", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}