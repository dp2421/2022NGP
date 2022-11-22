#include "stdafx.h"
#include "global.h"

#include "include.h"

HANDLE InputEvent;
CRITICAL_SECTION cs;
SESSION clients[3];

deque<Object*> objects;

queue<Client2ServerKeyActionPacket> messageQueue;

void ProcessPacket()
{
	// ���� ��� �Է� ó��
	while (!messageQueue.empty())
	{
		auto message = messageQueue.front();

		switch (message.key)
		{
		default:
			break;
		}
	}
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
	int retval;
	int index = *(int*)arg;

	SOCKET& sock = clients[index].socket;

	// �α��� ��Ŷ ���ú�
	Client2ServerLoginPacket login;
	retval = recv(sock, (char*)&login, sizeof(Client2ServerLoginPacket), MSG_WAITALL);
	if (retval == SOCKET_ERROR) { err_display("recv()"); return; }

	for(int i = 0; i <= index; ++i)
		SendLoginPacket(index);

	while (true)
	{
		// Ű �Է� ���ú�
		Client2ServerKeyActionPacket packet;
		retval = recv(sock, (char*)&packet, sizeof(Client2ServerKeyActionPacket), MSG_WAITALL);
		if (retval == SOCKET_ERROR) { err_display("recv()"); break; }

		messageQueue.push(packet);
	}
}

void StartCountDown()
{
	int timebuff = 0;
	auto countdown = std::chrono::high_resolution_clock::now();
	while (true)
	{
		int process = countTime - chrono::duration_cast<chrono::milliseconds>(countdown - std::chrono::high_resolution_clock::now()).count();
		if (process % 1000 != timebuff)
		{
			timebuff = process;
			SendCountdownPacket(timebuff + 1);
		}
	}

	SendGameStartPacket();
}

void InitializeGame()
{
	// InitalizeGame
}

void Update()
{
	Timer timer;
	while (true)
	{
		timer.Update();

		auto limit = std::chrono::high_resolution_clock::now() +
			chrono::duration_cast<chrono::nanoseconds>(chrono::milliseconds(16));
		while (std::chrono::high_resolution_clock::now() > limit) {}

		// packet ó��
		ProcessPacket();

		//UPDATE
	}
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
			clients[i].socket = client_sock;
				
			hThread = CreateThread(NULL, 0, InputThread,
				(LPVOID)i, 0, NULL);
			if (hThread == NULL) { closesocket(client_sock); }
			else 
			{ 
				CloseHandle(hThread);
				++i;
			}
		}
	}

	StartCountDown();

	InitializeGame();
	Update();

	closesocket(listen_sock);
	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}
