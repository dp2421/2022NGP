#include "stdafx.h"
#include "global.h"

#include "include.h"

HANDLE InputEvent;
CRITICAL_SECTION cs;

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
