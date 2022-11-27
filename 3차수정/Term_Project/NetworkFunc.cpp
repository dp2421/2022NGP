#include "stdafx.h"

// 서버 연결 변수
#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
SOCKET sock;

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
    int retval;

    Client2ServerLoginPacket c2sLogin;
    c2sLogin.size = sizeof(Client2ServerLoginPacket);
    c2sLogin.type = Client2ServerLogin;
    SendExpansion(sock, (char*)&c2sLogin, c2sLogin.size, 0);

    Server2ClientLoginPacket s2cLogin;
    RecvExpasion(sock, (char*)&s2cLogin, sizeof(Server2ClientLoginPacket), MSG_WAITALL);
}

DWORD WINAPI NetworkThread(LPVOID arg)
{
    cout << "Server Connct" << endl;

    // 로그인 보내기
    Login();

    // 서버에서 보내는 주기가 일정해서 계속 받아도 됨..아마
    while (true)
    {
        InfoOfPacket packet;
        RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);

        ProcessPacket(packet.size, packet.type);
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
        break;
    case Server2ClientGameClear:
        break;
    default:
        break;
    }
}

void InitMapInfo(int size)
{
    Server2ClientMapInfoPacket packet;
    int ret = RecvExpasion(sock, (char*)&packet, BUFFERSIZE, MSG_WAITALL);
    if (ret >= 0)
    {
        
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
    Server2ClientCountdownPacket packet;
    RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);

    packet.count; // Count
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
    // winmain 함수, winproc 함수 어찌저찌해서 시작? 근데 로비를 없앤 이상 안 만들어도 될듯
}

void InitPlayerInfo()
{

}
void InitObjectInfo()
{

}
void InitObstacleInfo()
{

}
void InputKey()
{
}
void SendKey()
{
    send(sock, player.dir, sizeof(int), 0);
}
void RecvPlayerPos()
{
    recv(sock, buf, sizeof(float), 0);
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
