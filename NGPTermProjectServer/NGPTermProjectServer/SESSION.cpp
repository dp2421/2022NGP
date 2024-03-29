#include "stdafx.h"
#include "SESSION.h"
#include "InteractionObject.h"
#include "GameManager.h"

void SESSION::DoSend(InfoOfPacket* info, void* packet)
{
	SendExpansion(this->socket, info, sizeof(InfoOfPacket), 0);
	if(packet != NULL)
		SendExpansion(this->socket, packet, info->size, 0);
}

int SESSION::DoRevc(void* packet, int size)
{
	ZeroMemory(this->buffer, sizeof(this->buffer));
	auto retval = RecvExpasion(this->socket, this->buffer, size, MSG_WAITALL);
	if (retval == -1)
	{
		return retval;
	}
	memcpy(packet, this->buffer, size);
	return retval;
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

void SESSION::SendMapInfoPacket()
{
	Server2ClientMapInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientMapInfoPacket);
	info.type = Server2ClientMapInfo;
	p.width = MAPWIDTH;
	p.height = MAPHEIGHT;
	memcpy(p.mapInfo, GameManager::GetInstance().Map, sizeof(GameManager::GetInstance().Map));

	this->DoSend(&info, &p);
}

void SESSION::SendInteractionObjectInfoPacket()
{
	ZeroMemory(this->sendBuffer, BUFFERSIZE);
	Server2ClientInteractionObjectInfoPacket p;
	InfoOfPacket info;

	info.size = GameManager::GetInstance().interactionObjects.size() * sizeof(Server2ClientInteractionObjectInfoPacket);
	info.type = Server2ClientInteractionObjectInfo;

	auto target = this->sendBuffer;
	for (auto& obj : GameManager::GetInstance().interactionObjects)
	{
		auto interactionObj = reinterpret_cast<InteractionObject*>(obj);
		p.ID = interactionObj->ID;
		p.x = interactionObj->pos.x;
		p.y = interactionObj->pos.y;
		p.type = (int)interactionObj->type;
		p.state = interactionObj->isInteraction;

		memcpy(target, &p, sizeof(p));
		target += sizeof(p);
	}

	this->DoSend(&info, this->sendBuffer);
}

void SESSION::SendPlayerInfoPacket(SESSION& player)
{
	Server2ClientPlayerInfoPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientPlayerInfoPacket);
	info.type = Server2ClientPlayerInfo;

	p.ID = player.ID;
	p.state = player.player.aniState;
	p.HP = player.player.HP;
	p.x = player.player.pos.x;
	p.y = player.player.pos.y;
	p.direction = player.player.direction;

	this->DoSend(&info, &p);
}

void SESSION::SendMonsterInfoPacket()
{
	ZeroMemory(this->sendBuffer, BUFFERSIZE);
	Server2ClientMonsterInfoPacket p;
	InfoOfPacket info;

	info.size = GameManager::GetInstance().monsters.size() * sizeof(Server2ClientMonsterInfoPacket);
	info.type = Server2ClientMonsterInfo;

	auto target = this->sendBuffer;
	for (auto& obj : GameManager::GetInstance().monsters)
	{
		auto monster = reinterpret_cast<Monster*>(obj);
		p.ID = monster->ID;
		p.HP = monster->HP;
		p.x = monster->pos.x;
		p.y = monster->pos.y;

		memcpy(target, &p, sizeof(p));
		target += sizeof(p);
	}

	this->DoSend(&info, this->sendBuffer);
}

void SESSION::SendBulletInfoPakcet()
{
	ZeroMemory(this->sendBuffer, BUFFERSIZE);
	Server2ClientBulletInfoPacket p;
	InfoOfPacket info;

	info.size = GameManager::GetInstance().bullets.size() * sizeof(Server2ClientBulletInfoPacket);
	info.type = Server2ClientBulletInfo;

	auto target = this->sendBuffer;
	for (auto& obj : GameManager::GetInstance().bullets)
	{
		auto bullet = reinterpret_cast<Bullet*>(obj);

		p.ID = bullet->ID;
		p.state = bullet->isActive;
		p.x = bullet->pos.x;
		p.y = bullet->pos.y;

		memcpy(target, &p, sizeof(p));
		target += sizeof(p);
	}

	this->DoSend(&info, this->sendBuffer);
}

void SESSION::SendObstacleInfoPacket()
{
	ZeroMemory(this->sendBuffer, BUFFERSIZE);
	Server2ClientObstacleInfoPacket p;
	InfoOfPacket info;

	info.size = GameManager::GetInstance().obstacles.size() * sizeof(Server2ClientObstacleInfoPacket);
	info.type = Server2ClientObstacleInfo;

	auto target = this->sendBuffer;
	for (auto& obj : GameManager::GetInstance().obstacles)
	{
		auto obstacle = reinterpret_cast<Obstacle*>(obj);

		p.ID = obstacle->ID;
		p.x = obstacle->pos.x;
		p.y = obstacle->pos.y;
		p.state = obstacle->isActive;

		memcpy(target, &p, sizeof(p));
		target += sizeof(p);
	}

	this->DoSend(&info, this->sendBuffer);
}

void SESSION::SendGameClearPacket(chrono::seconds time)
{
	Server2ClientGameClearPacket p;
	InfoOfPacket info;

	info.size = sizeof(Server2ClientGameClearPacket);
	info.type = Server2ClientGameClear;

	p.second = static_cast<int>(time.count());

	this->DoSend(&info, &p);
}

int RecvExpasion(SOCKET sock, void* buf, int len, int flage)
{
	int retval = recv(sock, reinterpret_cast<char*>(buf), len, flage);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
		return -1;
	}
	else return retval;
}

void SendExpansion(SOCKET sock, void* buf, int len, int flage)
{
	//cout << "SOCK : " << sock << " Size" << len << endl;
	int retval = send(sock, reinterpret_cast<char*>(buf), len, flage);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//if (len > BUFFERSIZE)
	//{
	//	while (len > 0)
	//	{
	//		int sendSize = len > BUFFERSIZE ? BUFFERSIZE : len;
	//		int retval = send(sock, (char*)&buf, sendSize, flage);
	//		if (retval == SOCKET_ERROR) {
	//			err_display("send()");
	//		}
	//
	//		len -= sendSize;
	//	}
	//}
	//else
	//{
	//	int sendSize = len > BUFFERSIZE ? BUFFERSIZE : len;
	//	int retval = send(sock, (char*)&buf, len, flage);
	//	if (retval == SOCKET_ERROR) {
	//		err_display("send()");
	//	}
	//}
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