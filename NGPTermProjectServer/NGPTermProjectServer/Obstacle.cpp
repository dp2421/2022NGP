#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle()
{
	this->Speed = 0;
	this->velocity = Vec2(0, 0);
	this->pos = Vec2(230, 650);
}

void Obstacle::Update(float deltaTime)
{
}


void Obstacle::InitObstacle()
{
//	for (int i = 0; i < OBS_GARO_COUNT; ++i) {
//		obs_garo[i].pos_x = (i / 2 + 1) * 1000 - 50;
//
//		if (i % 2 == 0)
//			obs_garo[i].rand_num = 100;
//		else if (i % 1 == 1)
//			obs_garo[i].rand_num = 400;
//	}
//
//	for (int i = 0; i < OBS_SERO_COUNT; ++i) {
//		obs_sero[i].pos_x = (i / 3 * 1000) + (i % 3) * 200;
//	}

	for (int i = 0; i < OBS_GARO_COUNT; ++i) {
		pos.x = (i / 2 + 1) * 1000 - 50;

		if (i % 2 == 0)
			obs_garo[i].rand_num = 100;
		else if (i % 1 == 1)
			obs_garo[i].rand_num = 400;
	}

	for (int i = 0; i < OBS_SERO_COUNT; ++i) {
		pos.x = (i / 3 * 1000) + (i % 3) * 200;
	}

}