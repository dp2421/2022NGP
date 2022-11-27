#include "stdafx.h"
// ���� ���� ����
#define SERVERPORT 9000
#define SERVERIP "127.0.0.1"
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

    cout << "Server Connct" << endl;

    // �α��� ������
    Client2ServerLoginPacket login;
    retval = send(sock, (char*)&login, sizeof(Client2ServerLoginPacket), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return;
    }
    retval = send(sock, (const char*)login.type, sizeof(Client2ServerLoginPacket), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return;
    }

    // Ű �Է� ������
    Client2ServerKeyActionPacket keyinput{ sizeof(Client2ServerKeyActionPacket), 1, keyinput.ID,'1' ,p_player.state };
    retval = send(sock, (char*)&keyinput, sizeof(Client2ServerKeyActionPacket), 0);
    if (retval == SOCKET_ERROR) {
        err_display("send()");
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
