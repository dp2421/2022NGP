#include "stdafx.h"
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

		messageQueue.pop();
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
		for (auto& obj : GameManager::GetInstance().monsters)
		{
			auto monster = reinterpret_cast<Monster*>(obj);
			//if(monster->HP > 0)
				cl.SendMonsterInfoPacket(monster);
		}
		// 총알 정보
		for (auto& obj : GameManager::GetInstance().bullets)
		{
			auto bullet = reinterpret_cast<Bullet*>(obj);
			//if (bullet->isActive)
				cl.SendBulletInfoPakcet(bullet);
		}
		// 장애물 정보

		for (auto& obj : GameManager::GetInstance().obstacles)
		{
			auto obstacle = reinterpret_cast<Obstacle*>(obj);
			//if (obstacle->isActive)
			//cl.SendObstacleInfoPacket();
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

	cout << "Server Initialize OK \n";
}

DWORD WINAPI InputThread(LPVOID arg)
{ 
	int retval;
	int index = *reinterpret_cast<int*>(arg);

	auto& client = GameManager::GetInstance().clients[index];
	client.ID = index;

	// 로그인 패킷 리시브
	Client2ServerLoginPacket login;
	if (client.DoRevc(&login, sizeof(Client2ServerLoginPacket)) == -1)
	{
		cout << "Login Fail" << endl;
		return 0;
	}

	client.SendLoginPacket();
	client.SendMapInfoPacket();

	while (true)
	{
		// 키 입력 리시브
		Client2ServerKeyActionPacket packet;
		retval = RecvExpasion(client.socket, (char*)&packet, sizeof(Client2ServerKeyActionPacket), MSG_WAITALL);

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

void Initialize()
{


	for (int i = 0; i < maxBulletCount; ++i)
	{
		GameManager::GetInstance().bullets.emplace_back();
	}
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
	int index;

	for (int i = 0; i < 3;)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); break; }
		else
		{
			index = i;
			GameManager::GetInstance().clients[index].socket = client_sock;
				
			hThread = CreateThread(NULL, 0, InputThread,
				reinterpret_cast<LPVOID>(&index), 0, NULL);
			if (hThread == NULL) { closesocket(client_sock); }
			else 
			{
				CloseHandle(hThread);
				++i;
			}
		}
	}

	StartCountDown();

	auto startTime = chrono::high_resolution_clock::now();
	Update();
	auto endTime = chrono::duration_cast<chrono::seconds>(startTime - chrono::high_resolution_clock::now());

	for (auto& cl : GameManager::GetInstance().clients)
		cl.SendGameClearPacket(endTime);

	closesocket(listen_sock);
	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}