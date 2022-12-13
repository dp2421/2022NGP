#pragma once
#include "Object.h"

class Player : public Object
{
public:
	int state;
	char aniState;

	bool isJump;
	bool isGround;

	int direction;
	int HP;
	Vec2 velocity;
public:
	Player();
	~Player();

	virtual void Update(float deltaTime) override;

	void ProccesInput(int key, bool pressed);

	void InitPlayer();
	void Jump();
	void Attack();
	void Interaction();
	void Damaged();
	void SetHorizontalVelocity(int state);
	void CollisionTile();
	void CollisionEnemy();
	void SetAniState();
private:
	int invincibleCount = 0;
	int AttackCooltimeCount = 0;
};