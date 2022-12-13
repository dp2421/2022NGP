#include "stdafx.h"
#include "Player.h"
#include "GameManager.h"
#include "Protocol.h"
#include "InteractionObject.h"

Player::Player()
{
	this->size.left = 8;
	this->size.right = 25;
	this->size.top = 5;
	this->size.bottom = 32;
	InitPlayer();
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	if (isPotal == true) return;

	this->pos.x += this->velocity.x * deltaTime;
	if (this->isGround == false)
	{
		this->velocity.y += PLAYER_GRAVITY;
		this->pos.y += this->velocity.y * deltaTime;
		if (this->pos.y > 900)
		{
			// 낙사
			InitPlayer();
		}
	}

	if ((this->state & (int)PlayerState::Attack) == (int)PlayerState::Attack)
	{
		Attack();
	}

	if (this->AttackCooltimeCount > 0)
	{
		this->AttackCooltimeCount--;
	}
	if (this->invincibleCount > 0)
	{
		this->invincibleCount--;
	}

	for (auto& object : GameManager::GetInstance().interactionObjects)
	{
		auto interactionObject = reinterpret_cast<InteractionObject*>(object);
		if(interactionObject->type == ObjectType::Potal)
			if (this->isCollision(interactionObject))
			{
				isPotal = true;
				this->pos = interactionObject->pos;
			}
	}

	CollisionEnemy();
	CollisionTile();

	SetAniState();
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
				this->velocity.x = 0;
			}
			else
			{
				SetHorizontalVelocity(this->state);
			}
		}
	}
		break;
		// 점프
	case VK_SPACE:
		if (pressed)
		{
			Jump();
		}
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
		if (pressed)
		{
			Interaction();
		}
		break;
	}
}

void Player::InitPlayer()
{
	this->pos = Vec2(230, playerStartY[this->ID]);
	this->velocity = Vec2(0, 0);
	this->direction = 1;
	this->state = (int)PlayerState::Idle;
	this->isJump = false;
	this->isGround = false;

	this->HP = 5;
}

void Player::Jump()
{
	// 점프
	if (!this->isJump)
	{
		this->state |= (int)PlayerState::Jump;
		this->isGround = false;
		this->isJump = true;
		this->velocity.y = -200;
	}
}

void Player::Attack()
{
	if (this->AttackCooltimeCount > 0)
	{
		return;
	}

	// 공격
	auto& bullets = GameManager::GetInstance().bullets;
	for (auto& bullet : bullets)
	{
		if (!reinterpret_cast<Bullet*>(bullet)->isActive)
		{
			reinterpret_cast<Bullet*>(bullet)->Shot(this->pos, direction);
			this->AttackCooltimeCount = 10;
			break;
		}
	}	
}

void Player::Interaction()
{
	// 상호작용
	// 레버 
	CollisionInteractionObejct();
}

void Player::Damaged()
{
	HP--;
	if (HP == 0)
	{
		// 죽음
		InitPlayer();
		return;
	}
	else
	{
		this->invincibleCount = 10;
	}
}

void Player::SetHorizontalVelocity(int state)
{
	if ((state & (int)PlayerState::Left) == (int)PlayerState::Left)
	{
		this->velocity.x = -playerSpeed;
		direction = -1;
	}
	else
	{
		this->velocity.x = playerSpeed;
		direction = 1;
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
	//for (int i = 0; i < MAPHEIGHT * MAPWIDTH; ++i)
	//{
	//	int y = i / 200;
	//	int x = i % 200;
	//	auto& tile = map[y][x];
	//	if (tile != 0)
	//	{
	//		if (this->isCollision(Vec2(x, y), blockRect))
	//		{
	//			if (y * BlockSize > this->pos.y + this->size.bottom - 10 && this->velocity.y >= 0)
	//			{
	//				if (this->isGround == false)
	//					state &= ~(int)PlayerState::Jump;
	//				this->pos.y = y * BlockSize - 32;
	//				this->velocity.y = 0;
	//				this->isGround = true;
	//			}
	//		}
	//	}
	//}

	bool collTile = false;

	for (int i = 0; i < MAPHEIGHT; ++i)
	{
		for (int j = 0; j < MAPWIDTH; ++j)
		{
			auto& tile = map[i][j];
			if (tile != 0)
			{
				if (tile == (int)TileType::Wall)
				{
					blockRect.bottom = 25;

					if (this->isCollision(Vec2(j * BlockSize, i * BlockSize), blockRect))
					{
						if (i * BlockSize > this->pos.y + this->size.bottom - 20 && this->velocity.y >= 0)
						{
							if (this->isJump == true)
								state &= ~(int)PlayerState::Jump;
							this->pos.y = i * BlockSize - 32;
							this->velocity.y = 0;
							this->isGround = true;
							this->isJump = false;
							
							collTile = true;
						}
					}
				}
				else
				{
					blockRect.bottom = 50;
					if (this->isCollision(Vec2(j * BlockSize, i * BlockSize), blockRect))
					{
						if (j * BlockSize > this->pos.x)
						{
							this->pos.x = j * BlockSize - this->size.right;
						}
						else
						{
							this->pos.x = j * BlockSize + BlockSize;
						}
					}
				}
			}
		}
	}

	if (!collTile) isGround = false;

	for (auto& object : GameManager::GetInstance().interactionObjects)
	{
		auto interactionObject = reinterpret_cast<InteractionObject*>(object);
		if (interactionObject->type == ObjectType::Door)
		{
			if (interactionObject->isInteraction == false)
			{
				if (this->isCollision(interactionObject))
				{
					if (interactionObject->pos.x > this->pos.x)
					{
						this->pos.x = interactionObject->pos.x - this->size.right;
					}
					else
					{
						this->pos.x = interactionObject->pos.x + interactionObject->size.right;
					}
				}
			}
		}
		else if (interactionObject->type == ObjectType::Button)
		{
			if (this->isCollision(interactionObject))
			{
				if (interactionObject->pos.y > this->pos.y + this->size.bottom - 10 && this->velocity.y >= 0)
				{
					if (this->isJump == true)
						state &= ~(int)PlayerState::Jump;
					this->pos.y = interactionObject->pos.y - 30;
					this->velocity.y = 0;
					this->isGround = true;
					this->isJump = false;
					interactionObject->collisionPlayer = this->ID;

					interactionObject->Interaction();
				}
			}
			else
			{
				if(interactionObject->isInteraction && interactionObject->collisionPlayer == this->ID)
					interactionObject->Interaction();
			}
		}
	}
}

void Player::CollisionEnemy()
{
	if (this->invincibleCount > 0)
	{
		return;
	}

	for (auto& monster : GameManager::GetInstance().monsters)
	{
		if (reinterpret_cast<Monster*>(monster)->HP <= 0) continue;
		if (this->isCollision(monster))
		{
			Damaged();
			return;
		}
	}

	auto& obstacles = GameManager::GetInstance().obstacles;
	for (auto& obstacle : obstacles)
	{
		if (this->isCollision(obstacle))
		{
			Damaged();
			return;
		}
	}
}

void Player::CollisionInteractionObejct()
{
	for (auto& object : GameManager::GetInstance().interactionObjects)
	{
		auto interactionObject = reinterpret_cast<InteractionObject*>(object);
		if (interactionObject->type == ObjectType::Door || interactionObject->type == ObjectType::Potal) continue;

		if (this->isCollision(object))
		{
			interactionObject->Interaction();
		}
		else
		{
			if (interactionObject->type == ObjectType::Button)
				interactionObject->Interaction();
		}
	}
}

void Player::SetAniState()
{
	if ((this->state & (int)PlayerState::Attack) == (int)PlayerState::Attack)
	{
		this->aniState = (char)STATE::ATTACK;
	}
	else if ((this->state & (int)PlayerState::Jump) == (int)PlayerState::Jump)
	{
		if (this->velocity.y > 0) this->aniState = (char)STATE::FALL;
		else this->aniState = (char)STATE::JUMP;
	}
	else if (this->velocity.x != 0)
	{
		this->aniState = (char)STATE::MOVE;
	}
	else
	{
		this->aniState = (char)STATE::IDLE;
	}
}