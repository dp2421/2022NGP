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
//Player player(300, 420, Manager::GetInstance().frog_idle);

unordered_map<int, Monster> monsters;
unordered_map<int, Bullet> bullets;

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

    // 서버에서 보내는 주기가 일정해서 계속 받아도 됨..아마
    while (true)
    {
        ZeroMemory(&socks.m_infoPack, sizeof(socks.m_infoPack));
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

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
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

void SendConnect()
{
    // 접속되면 handle정보 보냄
}

void StartCount()
{
    // 카운트를 세는 함수
    RecvExpasion(sock, &socks.m_cntPack, sizeof(socks.m_cntPack), MSG_WAITALL);

    // 5 4 3 2 1 순으로 카운팅
    socks.m_cntPack.count; // Count ??
}
void WaitStart()
{
    // 로딩중 띄워주면 됨
}

void RecvReady()
{

}

void GameStart()
{

}

void InitPlayerInfo()
{
    RecvExpasion(sock, &socks.m_playerPack, sizeof(socks.m_playerPack), MSG_WAITALL);
}

void InitObjectInfo()
{

}

void InitObstacleInfo()
{
    //Server2ClientObstacleInfoPacket packet;
    //RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);
    // help..
    //Server2ClientMapInfoPacket packet;
    //socks.m_mapPack;
    //int retval;
    //int ret = RecvExpasion(sock, (char*)&socks.m_mapPack, BUFFERSIZE, MSG_WAITALL);
    //if (ret >= 0)
    //{
    //    retval = RecvExpasion(sock, (char*)&ret, sizeof(int), 0);
    //    retval = RecvExpasion(sock, buf, ret, 0);
    //
    //    if (ret == 0)
    //    {
    //        cout << "Obstacle Recv Success";
    //        return;
    //    }
    //    else
    //    {
    //        // 뭔가의 처리
    //    }
    //}
    //else
    //{
    //    cout << "MapInfo Recv Fail";
    //    return;
    //}
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

    cout << "Send Key : " << key << " state " << socks.m_keyPack.state << endl;

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

void RecvPlayerPos()
{
    //int buf = 0;
    //RecvExpasion(sock, reinterpret_cast<char*>(buf), sizeof(buf), 0);
    //socks.m_playerPack.x = buf;
    //RecvExpasion(sock, reinterpret_cast<char*>(buf), sizeof(buf), 0);
    //socks.m_playerPack.y = buf;
}

void RecvPlayerInfo()
{
    auto& info = socks.m_playerPack;
    RecvExpasion(sock, &info, sizeof(info), 0);
    auto& player = players[info.ID];

    player.x = info.x;
    player.y = info.y;
    player.life = info.HP;
    player.state = (STATE)info.state;
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
    RecvExpasion(sock, &bulletbuffer, sizeof(size), 0);

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

void MonsterDead()
{

}

void ArrowDraw()
{

}

void Render()
{

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
    case Server2ClientTileInfo:
        // 타일 정보
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
        InitObstacleInfo();
        break;
    case Server2ClientGameClear:
        break;
    default:
        //cout << "Invalid Type : " << type << endl;
        break;
    }
}