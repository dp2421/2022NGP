#include "stdafx.h"
#include "global.h"

#include "include.h"

HANDLE InputEvent;
CRITICAL_SECTION cs;
SESSION clients[3];


void ProcessPacket()
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

void InitServer(SOCKET& target)
{
	int retval;

	target = socket(AF_INET, SOCK_STREAM, 0);
	if (target == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(target, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(target, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
}

DWORD WINAPI InputThread(LPVOID arg)
{

}

void StartCountDown()
{
	Timer startTimer;
	int timebuff = 0;
	while (true)
	{
		int process = countTime - startTimer.GetDeltaTimeMilli();
		if (process % 1000 != timebuff)
		{
			timebuff = process;
			SendCountdownPacket(timebuff + 1);
		}
	}

	SendGameStartPacket();
}

void Update()
{

}

int main(int argc, char* argv[])
{
	InitializeCriticalSection(&cs);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	SOCKET listen_sock;
	InitServer(listen_sock);

	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	for (int i = 0; i < 3;)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); break; }
		else
		{
			hThread = CreateThread(NULL, 0, InputThread,
				(LPVOID)client_sock, 0, NULL);
			if (hThread == NULL) { closesocket(client_sock); }
			else 
			{ 
				CloseHandle(hThread);
				clients[i].socket = client_sock;
				++i;
			}
		}
	}

	StartCountDown();

	Update();

	closesocket(listen_sock);
	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}
