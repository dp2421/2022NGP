#include "stdafx.h"
#include "global.h"
#include "include.h"

HANDLE InputEvent;
CRITICAL_SECTION cs;

queue<Client2ServerKeyActionPacket> messageQueue;

int loginCount = 0;

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
		cl.SendMonsterInfoPacket();
		// 총알 정보
		cl.SendBulletInfoPakcet();

		cl.SendObstacleInfoPacket();
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
	client.player.ID = index;

	// 로그인 패킷 리시브

	Client2ServerLoginPacket login;
	if (client.DoRevc(&login, sizeof(Client2ServerLoginPacket)) == -1)
	{
		cout << "Login Fail" << endl;
		return 0;
	}

	client.SendLoginPacket();
	client.SendPlayerInfoPacket(client);
	client.SendMapInfoPacket();


	EnterCriticalSection(&cs);
	loginCount++;
	LeaveCriticalSection(&cs);

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
	// 몬스터 초기화
	Vec2 MonsterPos[] =
	{
		Vec2(630, 670), 
		Vec2(700, 420),
		Vec2(400, 165), 
		Vec2(970, 420), 
		Vec2(1000, 670), 
		Vec2(2300, 670), 
		Vec2(2400, 420), 
		Vec2(2400, 165), 
		Vec2(2800, 165), 
		Vec2(4300, 670), 
		Vec2(4500, 165)
		//, (4750, 420), (5400, 420), (5550, 420), (5850, 420), (6150, 420), (6900, 420), 
		//(7050, 420), (7050, 165), (7050, 670), (7500, 420), (7500, 170), (7700, 670), (7800, 625), (7850, 420), 
		//(8100, 170), (8100, 625), (8300, 420), (8700, 370), (8800, 270), (8800, 720), (8950, 320), (8900, 165)
	};

	auto& monsters = GameManager::GetInstance().monsters;

	for (int i = 0; i < (sizeof(MonsterPos) / sizeof(POINT)); ++i)
	{
		monsters.emplace_back();
		auto& monster = monsters.back();
		monster = new Monster();
		monster->ID = i;
		monster->pos.x = MonsterPos[i].x;
		monster->pos.y = MonsterPos[i].y;
	}

	auto& bullets = GameManager::GetInstance().bullets;

	for (int i = 0; i < maxBulletCount; ++i)
	{
		bullets.emplace_back();
		auto& bullet = bullets.back();
		bullet = new Bullet();
		bullet->ID = i;
	}

	auto& obstacles = GameManager::GetInstance().obstacles;

	for (int i = 0; i < maxObstacleCount; ++i)
	{
		obstacles.emplace_back();
		auto& obstacle = obstacles.back();
		obstacle = new Obstacle();
		obstacle->ID = i;
	}
}

void Update()
{
	Timer timer;
	while (true)
	{
		if (loginCount < 3) continue;
		auto limit = timer.preTime +
			chrono::duration_cast<chrono::milliseconds>(chrono::milliseconds(34));
		while (std::chrono::high_resolution_clock::now() < limit) { Sleep(1); }

		timer.Update();

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
		ZeroMemory(&client_sock, sizeof(client_sock));
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) { err_display("accept()"); break; }
		else
		{
			index = i;
			GameManager::GetInstance().clients[index].socket = client_sock;
				
			hThread = CreateThread(NULL, 0, InputThread,
				reinterpret_cast<LPVOID>(&index), 0, NULL);
			if (hThread == NULL) {  }
			else 
			{
				CloseHandle(hThread);
				++i;
			}
		}
	}

	cout << "start";

	//StartCountDown();

	auto startTime = chrono::high_resolution_clock::now();

	Initialize();

	//StartCountDown();

	Update();
	auto endTime = chrono::duration_cast<chrono::seconds>(startTime - chrono::high_resolution_clock::now());

	for (auto& cl : GameManager::GetInstance().clients)
		cl.SendGameClearPacket(endTime);

	closesocket(listen_sock);
	DeleteCriticalSection(&cs);
	WSACleanup();
	return 0;
}