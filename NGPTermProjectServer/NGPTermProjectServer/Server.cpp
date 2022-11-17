#include <stdio.h>
#include <MSWSock.h>
#include <WS2tcpip.h>

#include "Protocol.h"

#define SERVERPORT 9000

HANDLE InputEvent;
CRITICAL_SECTION cs;

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

// 소켓 함수 오류 출력
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

class Vec2
{
	float x, y;
};

enum PlayerState
{
	Idle,
	Move,
	Attack,
	Dead,
	End
};

enum TileType
{
	Wall,
	Magma,
	Plate,
	Lever,
	Door,
	Potal,
	End
};

class Object
{
public:
	Vec2 vec;
	Vec2 size;
public:
	virtual void Update(float deltaTime) = 0;
	virtual bool isCollision(Object* rhs);
};

class Player : Object
{
public:
	PlayerState state;
	bool isJump;
	bool isGround;

	int HP;
	Vec2 velocity;
public:
	virtual void Update(float deltaTime) override;
	virtual bool isCollision(Object* rhs);

	void ProccesInput(int key);

	void Move();
	void Jump();
	void Interaction();
};

class Monster : Object
{
public:
	int HP;
	int direction;
	int speed;
public:
	virtual void Update(float deltaTime) override;
};

class Bullet : Object
{
	int direction;
	int speed;
public:
	virtual void Update(float deltaTime) override;
	virtual bool isCollision(Object* rhs) override;
};

class Tile : Object
{
public:
	TileType type;
	bool isInteraction;

	Object* linkedObject;
public:
	void Interaction();
	virtual bool isCollision(Object* rhs);
};

class Obstacle : Object
{
public:
	int Speed;
	Vec2 velocity;
public:
	virtual void Update(float deltaTime) override;
};

class SESSION
{
	int ID;
	SOCKET socket;
	Player player;

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
}

void SendGameStartPacket(int id)
{

}

void ProccesPacket()
{

}

void MovementLever()
{

}

bool StartCountdown()
{

}

float DeltaTime()
{

}

void InitServer(SOCKET& socket)
{

}

DWORD WINAPI UpdateThread(LPVOID arg)
{

}

DWORD WINAPI InputThread(LPVOID arg)
{

}

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&cs);
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	InputEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hThread = CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);
	if (hThread != NULL) CloseHandle(hThread);

	while (true)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); break; }

		hThread = CreateThread(NULL, 0, InputThread,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	closesocket(listen_sock);

	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}
