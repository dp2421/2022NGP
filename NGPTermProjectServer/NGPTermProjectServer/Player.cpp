#include "stdafx.h"
#include "Player.h"
#include "GameManager.h"
#include "Protocol.h"

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
	if (!this->isGround)
	{
		this->velocity.y += PLAYER_GRAVITY;
		this->pos.y += this->velocity.y * deltaTime;
	}
	CollisionTile();
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
			if ((this->state & reverse) == reverse)
			{
				this->state &= ~reverse;
			}

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
		if (pressed)
		{
			this->state |= (int)PlayerState::Attack;
		}
		else
		{
			this->state &= ~(int)PlayerState::Attack;
		}
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

void Player::CollisionTile()
{
	auto& map = GameManager::GetInstance().Map;
	RECT blockRect;
	blockRect.left = 0;
	blockRect.right = 50;
	blockRect.top = 0;
	blockRect.bottom = 50;
	// 타일 충돌
	for (int i = 0; i < MAPHEIGHT * MAPWIDTH; ++i)
	{
		auto& tile = map[i / 200][i % 200];
		if (tile != 0)
		{
			if (this->isCollision(Vec2((i % 200) * BlockSize, (i / 200) * BlockSize), blockRect))
			{
				//if (w_rect[i].top > player.collisionBox.bottom - 10 && player.velY >= 0)
				//{
				//	if (player.state == STATE::JUMP || player.state == STATE::FALL) {
				//		player.ChangeState(STATE::IDLE);
				//	}
				//	player.y = w_rect[i].top - 32;
				//	player.velY = 0;
				//	player.accY = 0;
				//	player.isRanding = true;
				//}
			}
		}
	}
}

void Player::CollisionMonster()
{
	//
}

void Player::CollisionObstacle()
{

}
