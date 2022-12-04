#pragma once
#include "stdafx.h"

// 서버 연결 변수
#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
SOCKET sock;
void ProcessPacket(int size, int type);

int RecvExpasion(SOCKET sock, char* buf, int len, int flage)
{
    int retval = recv(sock, (char*)&buf, len, flage);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return -1;
    }
    else return retval;
}

void SendExpansion(SOCKET sock, char* buf, int len, int flage)
{
    int retval = send(sock, (char*)&buf, len, flage);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
    }
}

void Login()
{
    socks.m_loginPack.size = sizeof(Client2ServerLoginPacket);
    socks.m_loginPack.type = Client2ServerLogin;
    SendExpansion(sock, (char*)&socks.m_loginPack, socks.m_loginPack.size, 0);

    RecvExpasion(sock, (char*)&socks.m_serverloginPack, sizeof(Server2ClientLoginPacket), MSG_WAITALL);
}

DWORD WINAPI NetworkThread(LPVOID arg)
{
    cout << "Server Connct" << endl;

    // 로그인 보내기
    Login();

    // 서버에서 보내는 주기가 일정해서 계속 받아도 됨..아마
    while (true)
    {
        RecvExpasion(sock, (char*)&socks.m_infoPack, sizeof(socks.m_infoPack), MSG_WAITALL);
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

    hThread = CreateThread(NULL, 0, NetworkThread,
        NULL, 0, NULL);
    if (hThread == NULL) { closesocket(sock); }
    else { CloseHandle(hThread); }
}

void InitMapInfo(int size)
{
    Server2ClientMapInfoPacket packet;
    int retval;
    char buf[BUFFERSIZE];
    int ret = RecvExpasion(sock, (char*)&packet, BUFFERSIZE, MSG_WAITALL);
    if (ret >= 0)
    {
        retval = RecvExpasion(sock, (char*)&ret, sizeof(int), 0);
        retval = RecvExpasion(sock, buf, ret, 0);
        if (ret == 0)
        {
            cout << "MapInfo Recv Success";
            return;
        }
        else
        {
            // 뭔가의 처리
        }
    }
    else
    {
        cout << "MapInfo Recv Fail";
        return;
    }
}

void SendConnect()
{
    // 접속되면 handle정보 보냄
}
void StartCount()
{
    // 카운트를 세는 함수
    RecvExpasion(sock, (char*)&socks.m_cntPack, sizeof(socks.m_cntPack), MSG_WAITALL);

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
    RecvExpasion(sock, (char*)&socks.m_playerPack, sizeof(socks.m_playerPack), MSG_WAITALL);
}
void InitObjectInfo()
{

}
void InitObstacleInfo()
{
    //Server2ClientObstacleInfoPacket packet;
    //RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);
    // help..
    Server2ClientMapInfoPacket packet;
    socks.m_mapPack;
    int retval;
    int ret = RecvExpasion(sock, (char*)&socks.m_mapPack, BUFFERSIZE, MSG_WAITALL);
    char buf[BUFFERSIZE];
    if (ret >= 0)
    {
        retval = RecvExpasion(sock, (char*)&ret, sizeof(int), 0);
        retval = RecvExpasion(sock, buf, ret, 0);

        if (ret == 0)
        {
            cout << "Obstacle Recv Success";
            return;
        }
        else
        {
            // 뭔가의 처리
        }
    }
    else
    {
        cout << "MapInfo Recv Fail";
        return;
    }
}
void InputKey()
{

}
void SendKey(int key)
{
    socks.m_keyPack.ID = socks.m_keyPack.ID;
    socks.m_keyPack.type = Client2ServerKeyAction;
    socks.m_keyPack.size = sizeof(Client2ServerKeyActionPacket);
    // 플레이어 어디선가 받아와야하는데 일단 나중ㅇ
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) //왼쪽
        socks.m_keyPack.key;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) //오른쪽
        socks.m_keyPack.key;
    if (GetAsyncKeyState(VK_UP) & 0x8000) //위
        socks.m_keyPack.key;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) //아래
        socks.m_keyPack.key;
    if (GetAsyncKeyState(0x41) & 0x8000)
        socks.m_keyPack.key;
    //packet.state = player.state;

    SendExpansion(sock, (char*)&socks.m_keyPack, sizeof(socks.m_keyPack), MSG_WAITALL);
}
void RecvPlayerPos()
{
    int buf = 0;
    RecvExpasion(sock, (char*)buf, sizeof(buf), 0);
    socks.m_playerPack.x = buf;
    RecvExpasion(sock, (char*)buf, sizeof(buf), 0);
    socks.m_playerPack.y = buf;
}
void RecvPlayerInfo()
{

}
void RecvBulletInfo()
{

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
        break;
    case Server2ClientMonsterInfo:
        break;
    case Server2ClientBulletInfo:
        break;
    case Server2ClientObstacleInfo:
        InitObstacleInfo();
        break;
    case Server2ClientGameClear:
        break;
    default:
        break;
    }
}