#include "stdafx.h"
#include "Player.h"

Player::Player()
{
	this->state = (int)PlayerState::Idle;
	this->isJump = false;
	this->isGround = false;

	this->HP = 5;
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	this->pos.x += this->velocity.x * deltaTime;
	if (false == this->isGround)
	{
		this->velocity.y -= PLAYER_GRAVITY;
		this->pos.y += this->velocity.y * deltaTime;
	}
}

void Player::ProccesInput(int key, bool pressed)
{
	switch (key)
	{
		// 이동
	case VK_LEFT:
	case VK_RIGHT:
	{
		int dir = key == VK_LEFT ? (int)PlayerState::Left : (int)PlayerState::Right;
		int reverse = key == VK_LEFT ? (int)PlayerState::Right : (int)PlayerState::Left;

		if (pressed)
		{
			this->state &= ~(int)PlayerState::Idle;
			this->state |= dir;

			SetHorizontalVelocity(dir);
		}
		else
		{
			this->state &= ~dir;
			if ((this->state & reverse) != reverse)
			{
				this->state |= (int)PlayerState::Idle;
			}
			else
			{
				SetHorizontalVelocity(this->state);
			}
		}
	}
		break;
	case VK_SPACE: // 점프
		this->state |= (int)PlayerState::Jump;
		break;
		// 공격
	case 'a':
	case 'A':
		this->state |= (int)PlayerState::Attack;
		break;
		// 상호작용
	case 'x':
	case 'X':
		Interaction();
		break;
	}
}

void Player::Jump()
{
	// 점프
}

void Player::Attack()
{
	// 공격
}

void Player::Interaction()
{
	// 상호작용
}

void Player::SetHorizontalVelocity(int state)
{
	if ((state & (int)PlayerState::Left) == (int)PlayerState::Left)
	{
		this->velocity.x = -playerSpeed;
	}
	else
	{
		this->velocity.x = playerSpeed;
	}
}
