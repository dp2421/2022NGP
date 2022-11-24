#include "stdafx.h"
#include "SESSION.h"

void SESSION::DoSend(void* packet)
{

}

void SESSION::DoRevc()
{

}

void SESSION::SendLoginPacket()
{
	Server2ClientLoginPacket p;
	p.size = sizeof(Server2ClientLoginPacket);
	p.type = Server2ClientLogin;
	p.ID = this->ID;
}

void SESSION::SendCountdownPacket(char cnt)
{
	Server2ClientCountdownPacket p;
	p.size = sizeof(Server2ClientCountdownPacket);
	p.type = Server2ClientCountdown;
	p.count = cnt;
}

void SESSION::SendGameStartPacket()
{
	Server2ClientGameStartPacket p;
	p.size = sizeof(Server2ClientGameStartPacket);
	p.type = Server2ClientGameStart;
}

void SESSION::SendMapInfoPacket(char* map)
{
	Server2ClientMapInfoPacket p;
	p.size = sizeof(Server2ClientMapInfoPacket);
	p.type = Server2ClientMapInfo;
	p.width = MAPWIDTH;
	p.height = MAPHEIGHT;
	memcpy(p.mapInfo, map, sizeof(map));
}

void SESSION::SendGameStartPacket()
{

}

void SESSION::SendMapInfoPacket()
{

}

void SESSION::SendTileInfoPacket()
{

}

void SESSION::SendPlayerInfoPacket()
{

}

void SESSION::SendMonsterInfoPacket()
{

}

void SESSION::SendBulletInfoPakcet()
{

}

void SESSION::SendObstacleInfoPacket()
{

}

void SESSION::SendGameClearPacket()
{

}
