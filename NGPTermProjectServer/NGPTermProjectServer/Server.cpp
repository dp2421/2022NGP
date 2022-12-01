#include "global.h"
#include "include.h"

HANDLE InputEvent;
CRITICAL_SECTION cs;

queue<Client2ServerKeyActionPacket> messageQueue;

void ProcessPacket()
{
	// 받은 모든 입력 처리
	while (!messageQueue.empty())
	{
		auto message = messageQueue.front();

		GameManager::GetInstance().clients[message.ID].player.ProccesInput(message.key, message.state);

		EnterCriticalSection(&cs);
		messageQueue.pop();
		LeaveCriticalSection(&cs);
	}
}

void SendPacket()
{
	for (auto& cl : GameManager::GetInstance().clients)
	{
		// 플레이어 정보
		for(auto& se : GameManager::GetInstance().clients)
			cl.SendPlayerInfoPacket(se);
		// 몬스터 정보
		cl.SendMonsterInfoPacket();
		// 총알 정보
		cl.SendBulletInfoPakcet();
		// 장애물 정보
		cl.SendObstacleInfoPacket();
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

	cout << "Server Initialize OK \n";
}

DWORD WINAPI InputThread(LPVOID arg)
{ 
	int retval;
	int index = *(int*)arg;

	SOCKET& sock = GameManager::GetInstance().clients[index].socket;

	// 로그인 패킷 리시브
	Client2ServerLoginPacket login;
	if (RecvExpasion(sock, (char*)&login, sizeof(Client2ServerLoginPacket), MSG_WAITALL) == -1)
		return 0;

	for(int i = 0; i <= index; ++i)
		GameManager::GetInstance().clients[index].SendLoginPacket();

	while (true)
	{
		// 키 입력 리시브
		Client2ServerKeyActionPacket packet;
		retval = RecvExpasion(sock, (char*)&packet, sizeof(Client2ServerKeyActionPacket), MSG_WAITALL);

		EnterCriticalSection(&cs);
		messageQueue.push(packet);
		LeaveCriticalSection(&cs);
	}

	return 0;
}

void StartCountDown()
{
	int timebuff = 0;
	auto countdown = std::chrono::high_resolution_clock::now();
	while (true)
	{
		int process = startCountdownTime - chrono::duration_cast<chrono::milliseconds>(countdown - std::chrono::high_resolution_clock::now()).count();
		if (process % 1000 != timebuff)
		{
			timebuff = process;
			for(auto& cl : GameManager::GetInstance().clients)
				cl.SendCountdownPacket(timebuff + 1);
		}
	}

	for (auto& cl : GameManager::GetInstance().clients)
		cl.SendGameStartPacket();
}

void InitializeGame()
{
	// InitalizeGame
	// MAP 정보 전달
	for (auto& se : GameManager::GetInstance().clients)
		se.SendMapInfoPacket(GameManager::GetInstance().Map[0]);
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

		// packet 처리
		ProcessPacket();
		
		for (auto& cl : GameManager::GetInstance().clients)
		{
			cl.player.Update(timer.GetDelteTime());
		}
		for (auto& obj : GameManager::GetInstance().monsters)
		{
			obj->Update(timer.GetDelteTime());
		}
		for (auto& obj : GameManager::GetInstance().bullets)
		{
			obj->Update(timer.GetDelteTime());
		}

		SendPacket();
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
			GameManager::GetInstance().clients[i].socket = client_sock;
				
			hThread = CreateThread(NULL, 0, InputThread,
				static_cast<LPVOID>(&i), 0, NULL);
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