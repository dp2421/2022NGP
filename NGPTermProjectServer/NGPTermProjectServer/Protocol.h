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

// 최초 로그인
struct Client2ServerLoginPacket
{
	unsigned char size;
	char	type;
};

// 플레이어 키 다운
struct Client2ServerKeyDownPacket
{
	unsigned char size;
	char	type;
	char	ID;
	int		key;
};

// 플레이어 키 업
struct Client2ServerKeyUpPacket
{
	unsigned char size;
	char	type;
	char	ID;
	int		key;
};

// 최초 로그인
struct Server2ClientLoginPacket
{
	unsigned char size;
	char	type;
	char	ID;
};

// 로비 카운트다운
struct Server2ClientCountdownPacket
{
	unsigned char size;
	char	type;
	char	count;
};

// 로비 -> 인게임
struct Server2ClientGameStartPacket
{
	unsigned char size;
	char	type;
};

// 맵 전체 정보
struct Server2ClientMapInfoPacket
{
	unsigned char size;
	char	type;

	short	width;
	short	height;

	char	mapInfo[MAPHEIGHT][MAPWIDTH];
};

// 단일 타일 정보
struct Server2ClientTileInfoPacket
{
	unsigned char size;
	char	type;

	short	x, y;
	char	state;
};

// 플레이어 정보
struct Server2ClienPlayerInfoPacket
{
	unsigned char size;
	char	type;

	char	ID;
	char	state;
	char	HP;
	float	x, y;
};

// 몬스터 정보
struct Server2ClientMonsterInfoPacket
{
	unsigned char size;
	char	type;

	char	HP;
	float	x, y;
};

// 총알 정보
struct Server2ClientBulletInfoPacket
{
	unsigned char size;
	char	type;

	float	x, y;
};

// 장애물 정보
struct Server2ClientObstacleInfoPacket
{
	unsigned char size;
	char	type;

	char	obstacleType;
	float	x, y;
};

// 게임 클리어, 경과 시간 = 점수
struct Server2ClientGameClearPacket
{
	unsigned char size;
	char	type;

	int		second;
};

#pragma pack (pop)