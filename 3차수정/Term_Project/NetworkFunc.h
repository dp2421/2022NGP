#pragma once
#include "stdafx.h"

// 서버 연결 변수
#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
SOCKET sock;
void ProcessPacket(int size, int type);
char buffer[5000];

const char attackKey = 'A';
const char InteractionKey = 'X';

Player players[3];

unordered_map<int, Monster> monsters;
unordered_map<int, Bullet> bullets;
unordered_map<int, Obstacle> obstacles;
unordered_map<int, Interaction_Object> interactionObjects;

int countDown = -1;

enum class KeyState : int
{
    LEFT        = 1 << 0,
    RIGHT       = 1 << 1,
    JUMP        = 1 << 2,
    ATTACK      = 1 << 3,
    INTERACTION = 1 << 4
};

int ID = -1;
int KeyInputBuffer;

int RecvExpasion(SOCKET sock, void* packet, int len, int flage)
{
    ZeroMemory(buffer, sizeof(buffer));
    int retval = recv(sock, buffer, len, flage);
    //cout << " Sock : " << sock << " len : " << len << " Recved Len : " << retval << endl;
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return -1;
    }
    memcpy(packet, buffer, len);
    return retval;
}

void SendExpansion(SOCKET sock, void* buf, int len, int flage)
{
    int retval = send(sock, reinterpret_cast<char*>(buf), len, flage);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
    }
}

void Login()
{
    socks.m_loginPack.size = sizeof(Client2ServerLoginPacket);
    socks.m_loginPack.type = Client2ServerLogin;
    SendExpansion(sock, &socks.m_loginPack, socks.m_loginPack.size, 0);

    ZeroMemory(&socks.m_infoPack, sizeof(socks.m_infoPack));
    RecvExpasion(sock, &socks.m_infoPack, sizeof(socks.m_infoPack), MSG_WAITALL);
    RecvExpasion(sock, &socks.m_serverloginPack, socks.m_infoPack.size, MSG_WAITALL);
    ID = socks.m_serverloginPack.ID;
    cout << ID;
}

DWORD WINAPI NetworkThread(LPVOID arg)
{
    cout << "Server Connct" << endl;

    // 로그인 보내기
    Login();

    while (true)
    {
        ZeroMemory(&socks, sizeof(socks));
        RecvExpasion(sock, &socks.m_infoPack, sizeof(socks.m_infoPack), MSG_WAITALL);
        ProcessPacket(socks.m_infoPack.size, socks.m_infoPack.type);
    }
}

void InitClient()
{
    int retval;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == INVALID_SOCKET)
        err_quit("socket() ");
        
    char ip[16] = {};
    cin >> ip;

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    HANDLE hThread;

    ZeroMemory(&socks, sizeof(socks));

    hThread = CreateThread(NULL, 0, NetworkThread, NULL, 0, NULL);
    if (hThread == NULL) { closesocket(sock); }
    else { CloseHandle(hThread); }
}

void InitMapInfo(int size)
{
    Server2ClientMapInfoPacket packet;
    RecvExpasion(sock, &packet, size, MSG_WAITALL);
    memcpy(Map, packet.mapInfo, packet.width * packet.height);
}

void StartCount()
{
    // 카운트를 세는 함수
    RecvExpasion(sock, &socks.m_cntPack, sizeof(socks.m_cntPack), MSG_WAITALL);
    // 5 4 3 2 1 순으로 카운팅
    countDown = socks.m_cntPack.count; // Count ??
}

void GameStart()
{
    countDown = -1;
}
    
void InitObstacleInfo(int size)
{
    char obstaclebuffer[BUFFERSIZE];
    RecvExpasion(sock, &obstaclebuffer, size, 0);

    Server2ClientObstacleInfoPacket* info = reinterpret_cast<Server2ClientObstacleInfoPacket*>(obstaclebuffer);
    for (int i = 0; i < size / sizeof(Server2ClientObstacleInfoPacket); ++i)
    {
        if (!obstacles.count(info[i].ID))
        {
            obstacles[info[i].ID] = Obstacle();
        }
        obstacles[info[i].ID].x = info[i].x;
        obstacles[info[i].ID].y = info[i].y;
        obstacles[info[i].ID].isActive = info[i].state;
    }

}

void RecvInteractionObjectInfo(int size)
{
    char InteractionObjectbuffer[BUFFERSIZE];
    RecvExpasion(sock, &InteractionObjectbuffer, size, 0);

    Server2ClientInteractionObjectInfoPacket* info = reinterpret_cast<Server2ClientInteractionObjectInfoPacket*>(InteractionObjectbuffer);
    for (int i = 0; i < size / sizeof(Server2ClientInteractionObjectInfoPacket); ++i)
    {
        if (!interactionObjects.count(info[i].ID))
        {
            interactionObjects[info[i].ID] = Interaction_Object();
        }
        interactionObjects[info[i].ID].x = info[i].x;
        interactionObjects[info[i].ID].y = info[i].y;
        interactionObjects[info[i].ID].type = (INTER_OBJ)info[i].type;
        interactionObjects[info[i].ID].state = info[i].state;
    }
}

void ProccesKey(int key, KeyState state)
{
    socks.m_keyPack.key = key;
    if ((GetAsyncKeyState(key) & 0x8000))
    {
        if ((KeyInputBuffer & (int)state) == (int)state) return;
        KeyInputBuffer |= (int)state;
        socks.m_keyPack.state = true;
    }
    else if ((KeyInputBuffer & (int)state) == (int)state)
    {
        KeyInputBuffer &= ~(int)state;
        socks.m_keyPack.state = false;
    }
    else return;

    SendExpansion(sock, &socks.m_keyPack, sizeof(socks.m_keyPack), 0);
}

void InputKey()
{
    socks.m_keyPack.ID = ID;
    socks.m_keyPack.type = Client2ServerKeyAction;
    socks.m_keyPack.size = sizeof(Client2ServerKeyActionPacket);

    ProccesKey(VK_LEFT, KeyState::LEFT);
    ProccesKey(VK_RIGHT, KeyState::RIGHT);
    ProccesKey(VK_SPACE, KeyState::JUMP);
    ProccesKey(attackKey, KeyState::ATTACK);
    ProccesKey(InteractionKey, KeyState::INTERACTION);
}

void RecvPlayerInfo()
{
    auto& info = socks.m_playerPack;
    RecvExpasion(sock, &info, sizeof(info), 0);
    auto& player = players[info.ID];

    player.x = info.x;
    player.y = info.y;
    player.life = info.HP;
    player.ChangeState((STATE)info.state);
    player.dir = info.direction;
}

void RecvMonsterInfo(int size)
{
    char monsterbuffer[BUFFERSIZE];
    RecvExpasion(sock, &monsterbuffer, size, 0);

    Server2ClientMonsterInfoPacket* info = reinterpret_cast<Server2ClientMonsterInfoPacket*>(monsterbuffer);
    for (int i = 0; i < size / sizeof(Server2ClientMonsterInfoPacket); ++i)
    {
        if (monsters.count(info[i].ID))
        {
            monsters[info[i].ID].x = info[i].x;
            monsters[info[i].ID].y = info[i].y;
            monsters[info[i].ID].life = info[i].HP;
        }
        else
        {
            monsters[info[i].ID] = Monster(info[i].x, info[i].y, Manager::GetInstance().monster_idle, MONSTER_TYPE::MONSTER_MOVE);
            monsters[info[i].ID].ChangeState(STATE::IDLE);
        }
    }
}

void RecvBulletInfo(int size)
{
    char bulletbuffer[BUFFERSIZE];
    RecvExpasion(sock, &bulletbuffer, size, 0);

    Server2ClientBulletInfoPacket* info = reinterpret_cast<Server2ClientBulletInfoPacket*>(bulletbuffer);
    for (int i = 0; i < size / sizeof(Server2ClientBulletInfoPacket); ++i)
    {
        if (!bullets.count(info[i].ID))
        {
            bullets[info[i].ID] = Bullet();
        }
        bullets[info[i].ID].x = info[i].x;
        bullets[info[i].ID].y = info[i].y;
        bullets[info[i].ID].isAttack = info[i].state;
    }
}

// 수신 패킷 처리
void ProcessPacket(int size, int type)
{
    //cout << "Type : " << type << endl;
    switch (type)
    {
    case Server2ClientCountdown:
        // 카운트다운
        StartCount();
        break;
    case Server2ClientGameStart:
        // 게임 시작, 추가적인 패킷 없음
        GameStart();
        break;
    case Server2ClientMapInfo:
        // 맵 정보
        InitMapInfo(size);
        break;
    case Server2ClientInteractionObjectInfo:
        RecvInteractionObjectInfo(size);
        // 상호작용 물체 정보
        break;
    case Server2ClientPlayerInfo:
        // 플레이어 정보
        RecvPlayerInfo();
        break;
    case Server2ClientMonsterInfo: // 몬스터 정보
        RecvMonsterInfo(size);
        break;
    case Server2ClientBulletInfo: // 총알 정보
        RecvBulletInfo(size);
        break;
    case Server2ClientObstacleInfo:
        InitObstacleInfo(size);
        break;
    case Server2ClientGameClear:
        break;
    default:
        //cout << "Invalid Type : " << type << endl;
        break;
    }
}