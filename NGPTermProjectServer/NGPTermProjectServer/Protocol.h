#pragma once

#define SERVERPORT 9000
#define MAPHEIGHT 20
#define MAPWIDTH 200

// Packet ID
constexpr char Client2ServerLogin = 0;
constexpr char Client2ServerKeyDown = 1;
constexpr char Client2ServerKeyUp = 2;

constexpr char Server2ClientLogin = 3;
constexpr char Server2ClientCountdown = 4;
constexpr char Server2ClientGameStart = 5;
constexpr char Server2ClientMapInfo = 6;
constexpr char Server2ClientTileInfo = 7;
constexpr char Server2ClienPlayerInfo = 8;
constexpr char Server2ClientMonsterInfo = 9;
constexpr char Server2ClientBulletInfo = 10;
constexpr char Server2ClientObstacleInfo = 11;
constexpr char Server2ClientGameClear = 12;

#pragma pack (push, 1)

// ���� �α���
struct Client2ServerLoginPacket
{
	unsigned char size;
	char	type;
};

// �÷��̾� Ű �ٿ�
struct Client2ServerKeyDownPacket
{
	unsigned char size;
	char	type;
	char	ID;
	int		key;
};

// �÷��̾� Ű ��
struct Client2ServerKeyUpPacket
{
	unsigned char size;
	char	type;
	char	ID;
	int		key;
};

// ���� �α���
struct Server2ClientLoginPacket
{
	unsigned char size;
	char	type;
	char	ID;
};

// �κ� ī��Ʈ�ٿ�
struct Server2ClientCountdownPacket
{
	unsigned char size;
	char	type;
	char	count;
};

// �κ� -> �ΰ���
struct Server2ClientGameStartPacket
{
	unsigned char size;
	char	type;
};

// �� ��ü ����
struct Server2ClientMapInfoPacket
{
	unsigned char size;
	char	type;

	short	width;
	short	height;

	char	mapInfo[MAPHEIGHT][MAPWIDTH];
};

// ���� Ÿ�� ����
struct Server2ClientTileInfoPacket
{
	unsigned char size;
	char	type;

	short	x, y;
	char	state;
};

// �÷��̾� ����
struct Server2ClienPlayerInfoPacket
{
	unsigned char size;
	char	type;

	char	ID;
	char	state;
	char	HP;
	float	x, y;
};

// ���� ����
struct Server2ClientMonsterInfoPacket
{
	unsigned char size;
	char	type;

	char	HP;
	float	x, y;
};

// �Ѿ� ����
struct Server2ClientBulletInfoPacket
{
	unsigned char size;
	char	type;

	float	x, y;
};

// ��ֹ� ����
struct Server2ClientObstacleInfoPacket
{
	unsigned char size;
	char	type;

	char	obstacleType;
	float	x, y;
};

// ���� Ŭ����, ��� �ð� = ����
struct Server2ClientGameClearPacket
{
	unsigned char size;
	char	type;

	int		second;
};

#pragma pack (pop)