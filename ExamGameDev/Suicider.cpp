#include "pch.h"
#include "Suicider.h"

#include "TextureManager.h"
#include "TimeObjectManager.h"
#include "SoundManager.h"
#include "StopwatchManager.h"

#include "Texture.h"
#include "Player.h"
#include "Level.h"
#include "Stopwatch.h"
#include "SoundEffect.h"
#include "utils.h"
#include <iostream>

Suicider::Suicider(const Point2f& position, float movementSpeed, size_t health, size_t attackDamage,
	TimeObjectManager* pTimeObjectManager, SoundManager* pSoundManager, TextureManager* pTextureManager) :
	BaseEnemy{
	Rectf{ position.x, position.y, 26.f, 17.f }, movementSpeed, 0, health, attackDamage,
		pTimeObjectManager, pSoundManager, pTextureManager
	},

	m_pSpawnStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(2.f) },

	m_pWalkTexture{ m_pTextureManager->CreateTexture("Enemies/120px-Risk_of_Rain_suicider.png","suiciderWalk") },
	m_pWalkStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(.15f) },
	m_CurrentWalkFrame{ 0 },
	m_WalkFrames{ 4 },

	m_pAttackTexture{ m_pTextureManager->CreateTexture("Enemies/SuiciderAttack4pxMarhin.png", "suiciderAttack") },
	m_CurrentAttackFrame{ 0 },
	m_AttackFrames{ 20 },

	m_pAttackStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(.08f) },
	m_SuiciderState{ SuiciderState::spawning },
	m_PreviousSuiciderState{ m_SuiciderState },

	m_pDeSpawnStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(5.f) },
	m_pDieStopwatch{ m_pEnemyStopwatchManager->CreateStopwatch(5.f) },

	m_pAttackSound{m_pSoundManager->CreateSoundEffect("suiciderExplosion", "Sounds/SoundEffects/wExplosiveShot.ogg") },
	m_pHitSound{m_pSoundManager->CreateSoundEffect("suiciderHit", "Sounds/SoundEffects/wBoarHit.ogg") }
{
	m_pSpawnStopwatch->Start();
}

Suicider::~Suicider()
{
	m_pTextureManager->DeleteTexture("suiciderAttack");
	m_pTextureManager->DeleteTexture("suiciderWalk");

	m_pSoundManager->DeleteSoundEffect("suiciderExplosion");
	m_pSoundManager->DeleteSoundEffect("suiciderHit");
}

void Suicider::Update(float elapsedSec, Level* pLevel, Player* pPlayer)
{
	BaseEnemy::Update(elapsedSec, pLevel, pPlayer);

	m_PreviousSuiciderState = m_SuiciderState;

	UpdateEnemyState(pPlayer);

	if (m_SuiciderState != m_PreviousSuiciderState)
	{
		UpdateShape();
	}

	UpdateEnemyBehavior(pPlayer, elapsedSec);
	pLevel->HandleCollision(m_Shape, m_Velocity, this, pPlayer);

	UpdateAnimationFrame();

	if (m_pDeSpawnStopwatch->IsTimeReached())
	{
		pLevel->DeSpawnEnemy(this);
	}
}

void Suicider::Draw() const
{
	if (m_IsFacingLeft)
	{
		glPushMatrix();
			glTranslatef(m_Shape.left + m_Shape.width / 2, 0, 0);
			glScalef(-1, 1, 1);
			glTranslatef(-(m_Shape.left + m_Shape.width / 2), 0, 0);
				DrawSuicider();
		glPopMatrix();
	}
	else
	{
		DrawSuicider();
	}

	BaseEnemy::DrawDroppedMoney();
}

void Suicider::TakeDamage(Player* pPlayer, size_t attackDamage)
{
	if (m_CurrentHealth == 0 || m_SuiciderState == SuiciderState::attacking)
	{
		return;
	}

	BaseEnemy::TakeDamage(pPlayer, attackDamage);
	m_pHitSound->Play(0);
}

const Rectf& Suicider::ChangeSourceRect() const
{
	constexpr float spaceBetweenAttackSprites{8.f};

	constexpr float walkSrcWidth{ 26.f };
	constexpr float walkSrcHeight{ 17.f };

	constexpr float attackSrcSize{ 41.f };

	Rectf srcRect{};

	switch (m_SuiciderState)
	{
	case SuiciderState::spawning:
		srcRect = Rectf{ 0 * walkSrcWidth,
						 0.f,
						 walkSrcWidth,
						 walkSrcHeight };
		break;

	case SuiciderState::walking:
		srcRect = Rectf{ m_CurrentWalkFrame * walkSrcWidth,
						 0,
						 walkSrcWidth ,
						 walkSrcHeight };
		break;

	case SuiciderState::attacking:
		srcRect = Rectf{ m_CurrentAttackFrame * (attackSrcSize + spaceBetweenAttackSprites),
						 m_pAttackTexture->GetHeight(), 
						 attackSrcSize,
						 attackSrcSize };
		break;
	}

	return srcRect;
}

void Suicider::UpdateEnemyState(Player* pPlayer)
{
	switch (m_SuiciderState)
	{
	case Suicider::SuiciderState::spawning:
		if (m_pSpawnStopwatch->IsTimeReached())
		{
			m_SuiciderState = SuiciderState::walking;
			m_pWalkStopwatch->Start();
			m_pDieStopwatch->Start();
		}
		break;

	case Suicider::SuiciderState::walking:
		if (utils::IsOverlapping(m_Shape, pPlayer->GetShape()))
		{
			m_SuiciderState = SuiciderState::attacking;
			m_pAttackStopwatch->Start();
			m_CurrentHealth = 0;
		}

		if (m_pDieStopwatch->IsTimeReached())
		{
			m_SuiciderState = SuiciderState::attacking;
			m_pAttackStopwatch->Start();
			m_CurrentHealth = 0;
		}
		break;

	case Suicider::SuiciderState::attacking:
		break;
	}
}

void Suicider::UpdateShape()
{
	constexpr float walkWidth{ 26.f };
	constexpr float walkHeight{ 17.f };

	constexpr float attackSize{ 53.f };

	switch (m_SuiciderState)
	{
	case Suicider::SuiciderState::spawning:
		break;

	case Suicider::SuiciderState::walking:
		m_Shape.width = walkWidth;
		m_Shape.height = walkHeight;
		break;

	case Suicider::SuiciderState::attacking:
		m_Shape.left = m_Shape.left + walkWidth / 2 - attackSize / 2;

		m_Shape.width = attackSize;
		m_Shape.height = attackSize;
		break;
	}
}

void Suicider::UpdateEnemyBehavior(Player* pPlayer, float elapsedSec)
{
	switch (m_SuiciderState)
	{
	case Suicider::SuiciderState::spawning:
		break;

	case Suicider::SuiciderState::walking:
		//Never stops moving until it explodes
		if (m_Shape.left + m_Shape.width < pPlayer->GetShape().left)
		{
			m_Velocity.x = m_MovementSpeed;
		}
		if (m_Shape.left > pPlayer->GetShape().left + pPlayer->GetShape().width)
		{
			m_Velocity.x = -m_MovementSpeed;
		}
		break;

	case Suicider::SuiciderState::attacking:
		m_Velocity.x = 0;
		Attack(pPlayer);
		break;
	}

	HandleMovement(elapsedSec);
}

void Suicider::UpdateAnimationFrame()
{
	if (m_Velocity.x == 0)
	{
		m_pWalkStopwatch->Reset();
		m_CurrentWalkFrame = 0;
	}
	else
	{
		m_pWalkStopwatch->Start();
	}

	if (m_pWalkStopwatch->IsTimeReached())
	{
		++m_CurrentWalkFrame %= m_WalkFrames;
	}

	if (m_pAttackStopwatch->IsTimeReached())
	{
		++m_CurrentAttackFrame;

		if (m_CurrentAttackFrame == m_AttackFrames - 5)
		{
			m_pAttackSound->Play(0);
		}
		if (m_CurrentAttackFrame == m_AttackFrames - 1)
		{
			m_pAttackStopwatch->Reset();
			m_pDeSpawnStopwatch->Start();
		}
	}
}

void Suicider::HandleMovement(float elapsedSec)
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

void Suicider::Attack(Player* pPlayer)
{
	if (m_CurrentAttackFrame == m_AttackFrames - 1)
	{
		if (!m_HasAttacked)
		{
			constexpr float blastRadius{ 25.f };

			if (utils::IsOverlapping(Rectf{m_Shape.left - blastRadius/2, 
										   m_Shape.bottom - blastRadius/2, 
										   m_Shape.width + blastRadius, 
										   m_Shape.height + blastRadius }
									, pPlayer->GetShape()))
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

void Suicider::DrawSuicider() const
{
	const Rectf srcRect{ ChangeSourceRect() };

	switch (m_SuiciderState)
	{
	case Suicider::SuiciderState::spawning:
		m_pWalkTexture->Draw(m_Shape, srcRect);
		break;

	case Suicider::SuiciderState::walking:
		m_pWalkTexture->Draw(m_Shape, srcRect);
		break;

	case Suicider::SuiciderState::attacking:
		m_pAttackTexture->Draw(m_Shape, srcRect);
		break;
	}

	BaseEnemy::DrawHealthBar();
}