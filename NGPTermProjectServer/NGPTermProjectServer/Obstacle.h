#pragma once
#include "Object.h"
#define OBS_GARO_COUNT 20
#define OBS_SERO_COUNT 36

// ���� ��ֹ� 2��
struct Obs_Garo {
	int pos_x;
	int pos_y;
	int rand_num;
};

// ���� ��ֹ� 3��
struct Obs_Sero {
	int pos_x;
	int pos_y;
	int rand_num;
};

static Obs_Garo obs_garo[OBS_GARO_COUNT];
static Obs_Sero obs_sero[OBS_SERO_COUNT];
static int garo_speed1 = 8;
static int garo_speed2 = 10;

class Obstacle : public Object
{
public:
	int Speed;
	Vec2 velocity;
	int moveCount;

public:
	Obstacle();
	virtual void Update(float deltaTime) override;
	void InitObstacle();
};