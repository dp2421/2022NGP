#include "stdafx.h"
SOCKET sock;
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
}

void SendConnect()
{
    // ���ӵǸ� handle���� ����
}
void StartCount()
{
    // ī��Ʈ�� ���� �Լ�
}
void WaitStart()
{

}
void RecvReady()
{

}
void GameStart()
{
    // winmain �Լ�, winproc �Լ� ���������ؼ� ����? �ٵ� �κ� ���� �̻� �� ���� �ɵ�
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

}
void RecvPlayerPos()
{

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
