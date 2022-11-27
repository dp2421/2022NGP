#include "stdafx.h"

// ���� ���� ����
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

    // �α��� ������
    Login();

    // �������� ������ �ֱⰡ �����ؼ� ��� �޾Ƶ� ��..�Ƹ�
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
    // ���ӵǸ� handle���� ����
}
void StartCount()
{
    // ī��Ʈ�� ���� �Լ�
    Server2ClientCountdownPacket packet;
    RecvExpasion(sock, (char*)&packet, sizeof(packet), MSG_WAITALL);

    packet.count; // Count
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
