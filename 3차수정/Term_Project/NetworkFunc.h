#pragma once
#include "stdafx.h"

// ���� ���� ����
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

enum class KeyState : int
{
    LEFT        = 1 << 0,
    RIGHT       = 1 << 1,
    JUMP        = 1 << 2,
    ATTACK      = 1 << 3,
    INTERACTION = 1 << 4
};

int ID;
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
}

DWORD WINAPI NetworkThread(LPVOID arg)
{
    cout << "Server Connct" << endl;

    // �α��� ������
    Login();

    // �������� ������ �ֱⰡ �����ؼ� ��� �޾Ƶ� ��..�Ƹ�
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

    hThread = CreateThread(NULL, 0, NetworkThread, NULL, 0, NULL);
    if (hThread == NULL) { closesocket(sock); }
    else { CloseHandle(hThread); }
}

void InitMapInfo(int size)
{
    Server2ClientMapInfoPacket packet;
    int retval;
    char buf[BUFFERSIZE];
    int ret = RecvExpasion(sock, &packet, size, MSG_WAITALL);
    if (ret > 0)
    {
        //retval = RecvExpasion(sock, (char*)&ret, sizeof(int), 0);
        //retval = RecvExpasion(sock, buf, ret, 0);
        //if (ret == 0)
        //{
        //    cout << "MapInfo Recv Success";
        //    return;
        //}
        //else
        //{
        //    // ������ ó��
        //}
        memcpy(Map, packet.mapInfo, packet.width * packet.height);
    }
    else
    {
        cout << "MapInfo Recv Fail";
        return;
    }
}

void SendConnect()
{
    // ���ӵǸ� handle���� ����
}

void StartCount()
{
    // ī��Ʈ�� ���� �Լ�
    RecvExpasion(sock, &socks.m_cntPack, sizeof(socks.m_cntPack), MSG_WAITALL);

    // 5 4 3 2 1 ������ ī����
    socks.m_cntPack.count; // Count ??
}
void WaitStart()
{
    // �ε��� ����ָ� ��
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
    //        // ������ ó��
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
    if ((GetAsyncKeyState(key) & 0x8000) && !((KeyInputBuffer & (int)state) == (int)state))
    {
        KeyInputBuffer |= (int)state;
        socks.m_keyPack.state = true;
    }
    else if (KeyInputBuffer & (int)state)
    {
        KeyInputBuffer &= ~(int)state;
        socks.m_keyPack.state = false;
    }
    else return;

    SendExpansion(sock, &socks.m_keyPack, sizeof(socks.m_keyPack), MSG_WAITALL);
}

void InputKey()
{
    socks.m_keyPack.ID = ID;
    socks.m_keyPack.type = Client2ServerKeyAction;
    socks.m_keyPack.size = sizeof(Client2ServerKeyActionPacket);

    ProccesKey(VK_LEFT, (int)KeyState::LEFT);
    ProccesKey(VK_RIGHT, (int)KeyState::RIGHT);
    ProccesKey(VK_SPACE, (int)KeyState::JUMP);
    ProccesKey(attackKey, (int)KeyState::ATTACK);
    ProccesKey(InteractionKey, (int)KeyState::INTERACTION);
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

void RecvMonsterInfo()
{
    auto& info = socks.m_monsterPack;
    RecvExpasion(sock, &info, sizeof(info), 0);

    monsters[info.ID].x = info.x;
    monsters[info.ID].y = info.y;
    monsters[info.ID].life = info.HP;
}

void RecvBulletInfo()
{
    auto& info = socks.m_bulletPack;
    RecvExpasion(sock, &info, sizeof(info), 0);

    bullets[info.ID].x = info.x;
    bullets[info.ID].y = info.y;
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

// ���� ��Ŷ ó��
void ProcessPacket(int size, int type)
{
    switch (type)
    {
    case Server2ClientCountdown:
        // ī��Ʈ�ٿ�
        StartCount();
        break;
    case Server2ClientGameStart:
        // ���� ����, �߰����� ��Ŷ ����
        GameStart();
        break;
    case Server2ClientMapInfo:
        // �� ����
        InitMapInfo(size);
        break;
    case Server2ClientTileInfo:
        // Ÿ�� ����
        break;
    case Server2ClientPlayerInfo:
        // �÷��̾� ����
        RecvPlayerInfo();
        break;
    case Server2ClientMonsterInfo: // ���� ����
        RecvMonsterInfo();
        break;
    case Server2ClientBulletInfo: // �Ѿ� ����
        RecvBulletInfo();
        break;
    case Server2ClientObstacleInfo:
        InitObstacleInfo();
        break;
    case Server2ClientGameClear:
        break;
    default:
        cout << "Invalid Type : " << type << endl;
        break;
    }
}