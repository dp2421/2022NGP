#pragma once
#include "stdafx.h"
class Sockets {
public:
	Client2ServerLoginPacket m_loginPack;
	Client2ServerKeyActionPacket m_keyPack;
	InfoOfPacket m_infoPack;
	Server2ClientLoginPacket m_serverloginPack;
	Server2ClientCountdownPacket m_cntPack;
	Server2ClientMapInfoPacket m_mapPack;
	Server2ClientInteractionObjectInfoPacket m_tilePack;
	Server2ClientPlayerInfoPacket m_playerPack;
	Server2ClientMonsterInfoPacket m_monsterPack;
	Server2ClientBulletInfoPacket m_bulletPack;
	Server2ClientObstacleInfoPacket m_obstaclePack;
	Server2ClientGameClearPacket m_winPack;
};