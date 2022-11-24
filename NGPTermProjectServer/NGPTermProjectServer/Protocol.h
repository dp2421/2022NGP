#pragma once

constexpr auto SERVERPORT = 9000;
constexpr auto MAPHEIGHT = 20;
constexpr auto MAPWIDTH = 200;

constexpr auto BUFFERSIZE = 1024;
constexpr auto MAXBULLET = 100;

// Packet ID
constexpr char Client2ServerLogin = 0;
constexpr char Client2ServerKeyAction = 1;

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

// 좌 우 스페이스 A, X

#pragma pack (push, 1)

// 최초 로그인
struct Client2ServerLoginPacket
{
	unsigned short size;
	char	type;
};

// 플레이어 키 입력
struct Client2ServerKeyActionPacket
{
	unsigned short size;
	char	type;
	char	ID;
	char	key;
	char	state;
};

struct InfoOfPacket
{
	unsigned short size;
	char	type;
};

// 최초 로그인
struct Server2ClientLoginPacket
{
	unsigned short size;
	char	type;
	char	ID;
};

// 로비 카운트다운
struct Server2ClientCountdownPacket
{
	unsigned short size;
	char	type;
	char	count;
};

// 로비 -> 인게임
struct Server2ClientGameStartPacket
{
	unsigned short size;
	char	type;
};

// 맵 전체 정보
struct Server2ClientMapInfoPacket
{
	unsigned short size;
	char	type;

	short	width;
	short	height;

	char	mapInfo[MAPHEIGHT][MAPWIDTH];
};

// 단일 타일 정보
struct Server2ClientTileInfoPacket
{
	unsigned short size;
	char	type;

	short	x, y;
	char	state;
};

// 플레이어 정보
struct Server2ClienPlayerInfoPacket
{
	unsigned short size;
	char	type;

	char	ID;
	char	state;
	char	HP;
	float	x, y;
};

// 몬스터 정보
struct Server2ClientMonsterInfoPacket
{
	unsigned short size;
	char	type;

	char	HP;
	float	x, y;
};

// 총알 정보
struct Server2ClientBulletInfoPacket
{
	unsigned short size;
	char	type;

	char	bulletSize;
	float	x[MAXBULLET], y[MAXBULLET];
};

// 장애물 정보
struct Server2ClientObstacleInfoPacket
{
	unsigned short size;
	char	type;

	char	obstacleType;
	float	x, y;
};

// 게임 클리어, 경과 시간 = 점수
struct Server2ClientGameClearPacket
{
	unsigned short size;
	char	type;

	int		second;
};

#pragma pack (pop)