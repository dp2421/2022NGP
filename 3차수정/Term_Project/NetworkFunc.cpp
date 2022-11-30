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
}

void InitPlayerInfo()
{
    Server2ClientPlayerInfoPacket packet;
    RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);
}
void InitObjectInfo()
{

}
void InitObstacleInfo()
{
    //Server2ClientObstacleInfoPacket packet;
    //RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);

    Server2ClientMapInfoPacket packet;
    int retval;
    int ret = RecvExpasion(sock, (char*)&packet, BUFFERSIZE, MSG_WAITALL);
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
    // 플레이어 받아오고 키도 따로 받아오는 방법으로 변경해야 할 듯.
    Client2ServerKeyActionPacket packet{};
    packet.ID = packet.ID; // this->ID ...
    packet.type = Client2ServerKeyAction;
    //packet.state = ;
    packet.size = sizeof(Client2ServerKeyActionPacket);
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) //왼쪽
        packet.key = VK_LEFT;
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) //오른쪽
        packet.key = VK_RIGHT;
    if (GetAsyncKeyState(VK_UP) & 0x8000) //위
        packet.key = VK_UP;
    if (GetAsyncKeyState(VK_DOWN) & 0x8000) //아래
        packet.key = VK_DOWN;
    if (GetAsyncKeyState(0x41) & 0x8000)
        packet.key = 0x41;
    //packet.state = player.state;

    SendExpansion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);
}
void RecvPlayerPos()
{
    // 이게맞 ????? 나 
    Server2ClientPlayerInfoPacket packet;
    int buf = 0;
    RecvExpasion(sock,(char*)buf,sizeof(buf),0);
    packet.x = buf;
    RecvExpasion(sock, (char*)buf, sizeof(buf), 0);
    packet.y = buf;
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