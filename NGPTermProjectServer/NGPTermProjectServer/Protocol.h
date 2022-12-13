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
constexpr char Server2ClientInteractionObjectInfo = 7;
constexpr char Server2ClientPlayerInfo = 8;
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
	bool	state;
};

struct InfoOfPacket
{
	unsigned short size;
	char	type;
};

// 최초 로그인
struct Server2ClientLoginPacket
{
	char	ID;
};

// 로비 카운트다운
struct Server2ClientCountdownPacket
{
	char	count;
};

// 맵 전체 정보
struct Server2ClientMapInfoPacket
{
	short	width;
	short	height;

	char	mapInfo[MAPHEIGHT][MAPWIDTH];
};

// 상호작용 물체 정보
struct Server2ClientInteractionObjectInfoPacket
{
	char	ID;
	short	x, y;
	char	type;
	bool	state;
};

// 플레이어 정보
struct Server2ClientPlayerInfoPacket
{
	char	ID;
	char	state;
	char	HP;
	char	direction;
	float	x, y;
};

// 몬스터 정보
struct Server2ClientMonsterInfoPacket
{
	char	ID;
	char	HP;
	float	x, y;
};

// 총알 정보
struct Server2ClientBulletInfoPacket
{
	char	ID;
	bool	state;
	float	x, y;
};

// 장애물 정보
struct Server2ClientObstacleInfoPacket
{
	char	ID;
	bool	state;
	float	x, y;
};

// 게임 클리어, 경과 시간 = 점수
struct Server2ClientGameClearPacket
{
	int		second;
};


#pragma pack (pop)