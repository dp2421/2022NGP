#pragma once
#include <atlimage.h>

class Manager
{
public:
	CImage player;
	CImage frog_idle;
	CImage frog_move;
	CImage frog_jump;
	CImage frog_fall;
	CImage frog_dead;
	CImage frog_attack;
	CImage rever_nomal;
	CImage rever_pull;
	CImage button_normal;
	CImage button_pull;
	CImage bg;
	CImage monster;
	CImage monster_idle;
	CImage monster_dead;
	CImage wall;
	CImage bullet;
	CImage effect;
	CImage magma;
	CImage garo;
	CImage sero;
	CImage My_heart;
	CImage portal_img;
	CImage FailGame;
	CImage SuccessGame;
	CImage Number;
public:
	static Manager& GetInstance()
	{
		static Manager instance;
		return instance;
	}
};