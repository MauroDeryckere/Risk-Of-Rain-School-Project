#include "pch.h"
#include "Parent.h"

#include "Texture.h"
#include "Player.h"
#include "Level.h"
#include "SoundManager.h"
#include "TextureManager.h"

#include "TimeObjectManager.h"
#include "StopwatchManager.h"
#include "Stopwatch.h"
#include "SoundEffect.h"

#include <cassert>
#include <iostream>

Parent::Parent(const Point2f& position, float movementSpeed, float jumpSpeed, unsigned int health, unsigned int attackDamage, TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager) :
	BaseEnemy{ Rectf{position.x, position.y, 48.f , 67.f} , movementSpeed, jumpSpeed, health , attackDamage, 
				pTimeObjectManager,  pSoundManager, pTextureManager },

	m_ParentState{ParentState::spawning},
	m_PreviousParentState{ ParentState::spawning },

	m_pTexture{ m_pTextureManager->CreateTexture("Enemies/Parent.png", "parentEnemy") },
	m_pSpawnStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(.5f) },
	m_CurrentSpawnFrame{ 0 },
	m_SpawnFrames{ 4 },

	m_pWalkStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(.2f) },
	m_CurrentWalkFrame{ 0 },
	m_WalkFrames{ 8 },

	m_pAttackStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(.2f) },
	m_CurrentAttackFrame{ 0 },
	m_AttackFrames{ 11 },

	m_pDyingStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(.5f) },
	m_CurrentDyingFrame{ 0 },
	m_DyingFrames{ 12 },

	m_pDeSpawnStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(5.f) },
	m_pAttackCooldownStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(2.f) }
{
	m_pSpawnStopwatch->Start();


	m_pDeathSoundEffect = m_pSoundManager->CreateSoundEffect("ParentDeath", "Sounds/SoundEffects/wChildDeath.ogg");
	m_pAttackSoundEffect = m_pSoundManager->CreateSoundEffect("ParentAttack", "Sounds/SoundEffects/wChildGShoot1.ogg");
	m_pHitSoundEffect = m_pSoundManager->CreateSoundEffect("ParentHit", "Sounds/SoundEffects/wChildHit.ogg");
}

Parent::~Parent()
{
	m_pTextureManager->DeleteTexture("parentEnemy");

	m_pSoundManager->DeleteSoundEffect("ParentDeath");
	m_pSoundManager->DeleteSoundEffect("ParentAttack");
	m_pSoundManager->DeleteSoundEffect("ParentHit");
}

void Parent::Update(float elapsedSec, Level* pLevel, Player* pPlayer)
{
	m_PreviousParentState = m_ParentState;

	const bool isOnGround{ pLevel->IsOnGround(m_Shape) };

	ChangeEnemyState(pPlayer);
	ChangeShape();
	HandleEnemyBehavior(pPlayer, elapsedSec);

	if (m_ParentState != ParentState::attacking)
	{
		pLevel->HandleCollision(m_Shape, m_Velocity);	
	}

	ChangeAnimationFrame();

	if (m_CurrentDyingFrame == m_DyingFrames - 1)
	{
		m_pDyingStopwatch->Reset();
		m_pDeSpawnStopwatch->Start();
	}
	if (m_pDeSpawnStopwatch->IsTimeReached() )
	{
		pLevel->DeSpawnEnemy(this);
		return;
	}
}

void Parent::Draw() const
{
	const Rectf sourceRect{ ChangeSourceRect() };

	if (m_IsFacingLeft)
	{
		glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, 0, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-(m_Shape.left + m_Shape.width / 2), 0, 0);
			m_pTexture->Draw(m_Shape, sourceRect);
			BaseEnemy::DrawHealthBar();
		glPopMatrix();
	}

	else
	{
		m_pTexture->Draw(m_Shape, sourceRect);
		BaseEnemy::DrawHealthBar();
	}
}

void Parent::TakeDamage(unsigned int attackDamage)
{
	BaseEnemy::TakeDamage(attackDamage);

	m_pHitSoundEffect->Play(0);
}

const Rectf Parent::ChangeSourceRect() const
{
	constexpr float spriteSheetOffset{ 1.f };

	Rectf sourceRect{};

	switch (m_ParentState)
	{
	case ParentState::spawning:
		sourceRect = Rectf{ spriteSheetOffset + m_CurrentSpawnFrame * (m_Shape.width + spriteSheetOffset) ,245.f, m_Shape.width, m_Shape.height };
		break;
	case ParentState::idle:
		sourceRect = Rectf{ spriteSheetOffset ,81.f, m_Shape.width, m_Shape.height };
		break;
	case ParentState::jumping:
		sourceRect = Rectf{ spriteSheetOffset ,163.f, m_Shape.width, m_Shape.height };
		break;
	case ParentState::walking:
		sourceRect = Rectf{ spriteSheetOffset + m_CurrentWalkFrame * (m_Shape.width + spriteSheetOffset),326.f, m_Shape.width, m_Shape.height };
		break;
	case ParentState::attacking:
		sourceRect = Rectf{ spriteSheetOffset + m_CurrentAttackFrame * (m_Shape.width + spriteSheetOffset), 405.f , m_Shape.width, m_Shape.height };
		break;
	case ParentState::dying:
		sourceRect = Rectf{ spriteSheetOffset + m_CurrentDyingFrame * (m_Shape.width + spriteSheetOffset), 553.f , m_Shape.width, m_Shape.height };
		break;
	}

	return sourceRect;
}

void Parent::ChangeShape()
{

	if (m_ParentState == ParentState::attacking)
	{
		m_Shape.width = 79.f;
		m_Shape.height = 67.f;
	}
	else if (m_ParentState == ParentState::dying)
	{
		m_Shape.width = 70.f;
		m_Shape.height = 65.f;
	}
	else
	{
		m_Shape.width = 48.f;
		m_Shape.height = 67.f;
	}
}

void Parent::ChangeEnemyState(Player* pPlayer)
{
	if (m_CurrentHealth == 0 || m_ParentState == ParentState::dying)
	{
		m_ParentState = ParentState::dying;
		return;
	}

	const Rectf& playerShape{ pPlayer->GetShape() };
	constexpr float radius{ 200.f };

	const bool isWithinTargetRange{ utils::GetDistance(Point2f{m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2},
													   Point2f{playerShape.left + playerShape.width/2, playerShape.bottom + playerShape.height/2}) <= radius };

	const bool isWithinAttackRange{ utils::IsOverlapping(pPlayer->GetShape(), m_Shape) };

	const bool isOnAttackCoolDown{ m_pAttackCooldownStopwatch->IsRunning() };

	switch (m_ParentState)
	{
	case ParentState::idle:
		if (isWithinTargetRange)
		{
			m_ParentState = ParentState::walking;
		}
		if (isWithinAttackRange && !isOnAttackCoolDown) 
		{
			m_ParentState = ParentState::attacking;
		}
		break;

	case ParentState::jumping: //TODO
		break;

	case ParentState::walking:
		if (!isWithinTargetRange)
		{
			m_ParentState = ParentState::idle;
		}
		if (isWithinAttackRange && !isOnAttackCoolDown)
		{
			m_ParentState = ParentState::attacking;
		}
		break;

	case ParentState::attacking:
		if (isOnAttackCoolDown)
		{
			m_ParentState = ParentState::idle;
		}
		break;
	}


}

void Parent::HandleEnemyBehavior(Player* pPlayer, float elapsedSec)
{
	switch (m_ParentState)
	{
	case ParentState::idle:

		break;

	case ParentState::jumping:
		break;

	case ParentState::walking:
		if (m_Shape.left + m_Shape.width < pPlayer->GetShape().left)
		{
			m_Velocity.x = m_MovementSpeed;
		}
		else if (m_Shape.left > pPlayer->GetShape().left + pPlayer->GetShape().width)
		{
			m_Velocity.x = -m_MovementSpeed;
		}

		Move(elapsedSec);
		break;

	case ParentState::attacking:
		Attack(pPlayer);
		break;

	case ParentState::dying:
		break;
	}

	//HandleMovement
}

void Parent::ChangeAnimationFrame()
{
	ResetAnimationStopwatches();

	if (m_pAttackCooldownStopwatch->IsTimeReached())
	{
		m_pAttackCooldownStopwatch->Reset();
	}

	switch (m_ParentState)
	{
	case ParentState::spawning:
		ChangeSpawnAnimationFrame();
		break;
	case ParentState::walking:
		ChangeWalkAnimationFrame();
		break;
	case ParentState::attacking:
		ChangeAttackAnimationFrame();
		break;
	case ParentState::dying:
		ChangeDyingAnimationFrame();
		break;
	default:
		break;
	}
}

void Parent::ResetAnimationStopwatches()
{
	if (m_ParentState != ParentState::spawning)
	{
		m_pSpawnStopwatch->Reset();
	}
	if (m_ParentState != ParentState::walking)
	{
		m_pWalkStopwatch->Reset();
		m_CurrentWalkFrame = 0;
	}
	if (m_ParentState != ParentState::attacking)
	{
		m_pAttackStopwatch->Reset();
		m_CurrentAttackFrame = 0;
	}
}

void Parent::ChangeSpawnAnimationFrame()
{
	if (m_pSpawnStopwatch->IsTimeReached() && m_ParentState == ParentState::spawning)
	{
		++m_CurrentSpawnFrame;

		if (m_CurrentSpawnFrame == m_SpawnFrames - 1)
		{
			m_ParentState = ParentState::idle;
			m_pSpawnStopwatch->Stop();
		}
	}
}

void Parent::ChangeWalkAnimationFrame()
{
	m_pWalkStopwatch->Start();

	if (m_pWalkStopwatch->IsTimeReached())
	{
		++m_CurrentWalkFrame %= m_WalkFrames;
	}
}

void Parent::ChangeAttackAnimationFrame()
{
	m_pAttackStopwatch->Start();

	if (m_pAttackStopwatch->IsTimeReached())
	{
		++m_CurrentAttackFrame;

		if (m_CurrentAttackFrame == 1)
		{
			m_pAttackSoundEffect->Play(0);
		}

		if (m_CurrentAttackFrame == m_AttackFrames)
		{
			m_pAttackCooldownStopwatch->Start();

			m_pAttackStopwatch->Reset();

			m_CurrentAttackFrame = 0;
		}

	}
}

void Parent::ChangeDyingAnimationFrame() 
{
	if (!m_pDeSpawnStopwatch->IsRunning())
	{
		m_pDyingStopwatch->Start();

		if (m_pDyingStopwatch->IsTimeReached())
		{
			if (m_CurrentDyingFrame == 1)
			{
				m_pDeathSoundEffect->Play(0);
			}

			++m_CurrentDyingFrame %= m_DyingFrames;
		}
	}
}

void Parent::Attack(Player* pPlayer)
{
	if (m_CurrentAttackFrame == m_AttackFrames-1 )
	{
		if (!m_HasAttacked)
		{
			if (utils::IsOverlapping(m_Shape, pPlayer->GetShape()))
			{
				pPlayer->TakeDamage(m_AttackDamage);
			}
			m_HasAttacked = true;
		}
	}
	else
	{
		m_HasAttacked = false;
	}
}

void Parent::Move(float elapsedSec)
{
	if (m_Velocity.x < 0.f)
	{
		m_IsFacingLeft = true;
	}
	else if (m_Velocity.x > 0.f)
	{
		m_IsFacingLeft = false;
	}

	m_Shape.left += m_Velocity.x * elapsedSec;

	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}
	